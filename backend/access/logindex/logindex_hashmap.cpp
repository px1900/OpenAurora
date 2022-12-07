#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <iostream>
#include "access/logindex_hashmap.h"
#include <atomic>


//typedef boost::shared_mutex Lock;
//typedef boost::unique_lock< Lock >  WriterLock;
//typedef boost::shared_lock< Lock >  ReaderLock;

//Lock myLock;


HashMap relSizeHashMap;
HashMap pageVersionHashMap;

void HashMapInit(HashMap *hashMap_p, int bucketNum) {
    *hashMap_p = (HashMap) malloc(sizeof(struct HashMapStruct) );
#ifdef ENABLE_DEBUG_INFO
    printf("%s malloc %lu for HashMapStruct\n", __func__ , sizeof(struct HashMapStruct));
#endif
    (*hashMap_p)->bucketNum = bucketNum;
    (*hashMap_p)->bucketList = (HashBucket*) malloc(bucketNum * sizeof(HashBucket));

    for(int i = 0; i < bucketNum; i++) {
        (*hashMap_p)->bucketList[i].nodeList = NULL;
        pthread_rwlock_init(&(*hashMap_p)->bucketList[i].bucketLock, NULL);
    }
#ifdef ENABLE_DEBUG_INFO
    printf("Hashmap Address = %p\n", (*hashMap_p));
    printf("%s finished \n", __func__ );
    fflush(stdout);
#endif
}

uint32_t HashKey(KeyType key) {
//    return tag_hash((void*) &key, sizeof(KeyType));
    uint32_t res = 0;
    res += (key.SpcID&0xFF) * 13 + 7;
    res += (key.DbID&0xFF) * 17 + 5;
    res += (key.RelID&0xFF) * 11 + 7;
    res += (key.ForkNum&0xFF) * 3 + 29;

    if(key.BlkNum != -1) {
        res += (key.BlkNum&0xFF) * 7 + 11;
    }
//    res |= (key.SpcID&0xFF) * 13;
//    res <<= 7;
//    res |= (key.DbID&0xFF) * 17;
//    res <<= 5;
//    res |= (key.RelID&0xFF) * 11;
//    res <<= 7;
//    res |= (key.ForkNum&0xFF) * 3;
    return res;
}

bool KeyMatch(KeyType key1, KeyType key2) {
    if(key1.SpcID == key2.SpcID
    && key1.DbID == key2.DbID
    && key1.RelID == key2.RelID
    && key1.ForkNum == key2.ForkNum
    && key1.BlkNum == key2.BlkNum)
        return true;
    else
        return false;
}

// Update header's first element's lsn
// Also update header's replayedLsn
bool HashMapUpdateFirstEmptySlot(HashMap hashMap, KeyType key, uint64_t lsn) {
    uint32_t hashValue = HashKey(key);
    uint32_t bucketPos = hashValue % hashMap->bucketNum;

    pthread_rwlock_wrlock(&hashMap->bucketList[bucketPos].bucketLock);

    HashNodeHead* iter = hashMap->bucketList[bucketPos].nodeList;
    bool foundHead = false;
    while(iter != NULL) {
        // If found matched key, break the loop
        if(iter->hashValue == hashValue
           && KeyMatch(iter->key, key)) {
            foundHead = true;
            break;
        }

        iter = iter->nextHead;
    }

    if (!foundHead) {
        pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
        return false;
    }


    pthread_rwlock_wrlock(&iter->headLock);
    if (iter->lsnEntry[0].lsn == 0) {
        iter->lsnEntry[0].lsn = lsn;

        if(iter->replayedLsn < lsn) {
            iter->replayedLsn = lsn;
        }
        pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
        pthread_rwlock_unlock(&iter->headLock);
        return true;
    } else {
        pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
        pthread_rwlock_unlock(&iter->headLock);
        return false;
    }
}

bool HashMapUpdateReplayedLsn(HashMap hashMap, KeyType key, uint64_t lsn, bool holdHeadLock) {
    uint32_t hashValue = HashKey(key);
    uint32_t bucketPos = hashValue % hashMap->bucketNum;

    pthread_rwlock_wrlock(&hashMap->bucketList[bucketPos].bucketLock);

    HashNodeHead* iter = hashMap->bucketList[bucketPos].nodeList;
    bool foundHead = false;
    while(iter != NULL) {
        // If found matched key, break the loop
        if(iter->hashValue == hashValue
           && KeyMatch(iter->key, key)) {
            foundHead = true;
            break;
        }

        iter = iter->nextHead;
    }

    if (!foundHead) {
        pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
        return false;
    }


    if(!holdHeadLock)
        pthread_rwlock_wrlock(&iter->headLock);
    if (iter->replayedLsn < lsn) {
        iter->replayedLsn = lsn;
        pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
        pthread_rwlock_unlock(&iter->headLock);
        return true;
    } else {
        pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
        pthread_rwlock_unlock(&iter->headLock);
        return false;
    }
}

bool HashMapInsertKey(HashMap hashMap, KeyType key, uint64_t lsn, int pageNum, bool noEmptyFirstSlot) {
    uint32_t hashValue = HashKey(key);
    uint32_t bucketPos = hashValue % hashMap->bucketNum;

#ifdef ENABLE_DEBUG_INFO
    printf("%s bucketPos = %u, hashValue = %u\n", __func__ , bucketPos, hashValue);
    fflush(stdout);
#endif
    // Lock this slot
    // Maybe we won't add a new head, and only need a ReadLock
    // TODO: use readLock first, if we need create head, writeLock it later
//    WriterLock w_lock(hashMap->bucketList[bucketPos].bucketLock);
    pthread_rwlock_wrlock(&hashMap->bucketList[bucketPos].bucketLock);

#ifdef ENABLE_DEBUG_INFO
    printf("%s get bucket lock\n", __func__ );
    fflush(stdout);
#endif

    HashNodeHead* iter = hashMap->bucketList[bucketPos].nodeList;
    bool foundHead = false;
    while(iter != NULL) {

#ifdef ENABLE_DEBUG_INFO
        printf("%s %d\n", __func__ , __LINE__);
        fflush(stdout);
#endif
        // If found matched key, break the loop
        if(iter->hashValue == hashValue
        && KeyMatch(iter->key, key)) {
            foundHead = true;
            break;
        }

        iter = iter->nextHead;
    }

#ifdef ENABLE_DEBUG_INFO
    printf("%s %d\n", __func__ , __LINE__);
    fflush(stdout);
#endif

    // If no head matches in this slot, crate a new head node
    if(!foundHead) {
#ifdef ENABLE_DEBUG_INFO
        printf("Create head node\n");
#endif
        HashNodeHead* head = (HashNodeHead*) malloc(sizeof(HashNodeHead));

        head->key = key;
        head->hashValue = hashValue;
        head->bucket = &hashMap->bucketList[bucketPos];

        pthread_rwlock_init(&head->headLock, NULL);

        if(key.BlkNum != -1 && !noEmptyFirstSlot) { // this is for page version lsn list
            head->lsnEntry[0].lsn = 0;
            head->lsnEntry[1].lsn = lsn;
            head->entryNum = 2;
        } else{ // this is for rel nblocks
            head->lsnEntry[0].lsn = lsn;
            head->lsnEntry[0].pageNum = pageNum;
            head->entryNum = 1;
        };
        head->maxLsn = lsn;
        // If toReplayLsn == 0, then no page was replayed
        head->replayedLsn = 0;

        head->nextHead = NULL;
        head->nextEle = NULL;
        head->tailEle = NULL;

        // Add this new head to the first position of bucket list
        head->nextHead = head->bucket->nodeList;
        head->bucket->nodeList = head;

        pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
        return true;
    }

    pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
    // Now, we found the matched head node in this slot

    // First, lock this header
#ifdef ENABLE_DEBUG_INFO
    printf("%s try to get  header lock\n", __func__ );
#endif
//    WriterLock w_header_lock(iter->headLock);
    pthread_rwlock_rdlock(&iter->headLock);

#ifdef ENABLE_DEBUG_INFO
    printf("%s Get header lock\n", __func__ );
    fflush(stdout);
#endif

    // If this lsn is smaller than the maximum lsn, do nothing
    if(iter->maxLsn > lsn) {
#ifdef ENABLE_DEBUG_INFO
        printf("%s try to insert failed, logindex_maxLsn = %lu, parameter lsn = %lu\n", __func__ , iter->maxLsn, lsn);
        fflush(stdout);
#endif

//        pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
        pthread_rwlock_unlock(&iter->headLock);
        return false;
    }

    // In the same transaction, RpcCreate, RpcExtend will update page number
    // If this happened, just update the page number in place
    if(iter->maxLsn == lsn) {
#ifdef ENABLE_DEBUG_INFO
        printf("%s %d\n", __func__ , __LINE__);
        fflush(stdout);
#endif
        // the maxLsn should always be the tail element in this head's list

        // Check whether it's in the head
        if(iter->entryNum <= HASH_HEAD_NUM && iter->nextEle == NULL) {
#ifdef ENABLE_DEBUG_INFO
            printf("%s %d, found in head entryPos = %d\n", __func__ , __LINE__, iter->entryNum-1);
            fflush(stdout);
#endif

            iter->lsnEntry[iter->entryNum-1].pageNum = pageNum;
        } else { // Check the tail node in this head list
#ifdef ENABLE_DEBUG_INFO
            printf("%s %d, found in node entryPos = %d\n", __func__ , __LINE__, iter->tailEle->entryNum-1);
            fflush(stdout);
#endif

            iter->tailEle->lsnEntry[ iter->tailEle->entryNum -1 ].pageNum = pageNum;
        }

//        pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
        pthread_rwlock_unlock(&iter->headLock);
        return true;
    }

#ifdef ENABLE_DEBUG_INFO
    printf("%s %d\n", __func__ , __LINE__);
    fflush(stdout);
#endif
    // If head still has available space, add it to head
    if(iter->entryNum < HASH_HEAD_NUM) {
#ifdef ENABLE_DEBUG_INFO
        printf("%s %d\n", __func__ , __LINE__);
        fflush(stdout);
#endif
        iter->lsnEntry[iter->entryNum].pageNum = pageNum;
        iter->lsnEntry[iter->entryNum].lsn = lsn;
        iter->entryNum++;

        iter->maxLsn = lsn;

//        pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
        pthread_rwlock_unlock(&iter->headLock);
        return true;
    }

#ifdef ENABLE_DEBUG_INFO
    printf("%s %d\n", __func__ , __LINE__);
    fflush(stdout);
#endif
    // If the tail node has available space, add this entry to tail node
    if(iter->tailEle
        && iter->tailEle->entryNum < HASH_ELEM_NUM) {

#ifdef ENABLE_DEBUG_INFO
        printf("%s %d\n", __func__ , __LINE__);
        fflush(stdout);
//        printf("add into tail node\n");
#endif
        HashNodeEle *nodeEle = iter->tailEle;
        nodeEle->lsnEntry[nodeEle->entryNum].pageNum = pageNum;
        nodeEle->lsnEntry[nodeEle->entryNum].lsn = lsn;

        nodeEle->entryNum++;

        // update this node's maxLsn and header's lsn
        nodeEle->maxLsn = lsn;
        iter->maxLsn = lsn;

//        pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
        pthread_rwlock_unlock(&iter->headLock);
        return true;
    }

#ifdef ENABLE_DEBUG_INFO
    printf("%s %d\n", __func__ , __LINE__);
    fflush(stdout);
#endif
    // If the tail node doesn't have enough space or there is no element node
    // Add a new element node to the head list tail.

//    printf("Create tail node\n");
    HashNodeEle* eleNode = (HashNodeEle*) malloc(sizeof(HashNodeEle));
    eleNode->maxLsn = lsn;
    eleNode->lsnEntry[0].pageNum = pageNum;
    eleNode->lsnEntry[0].lsn = lsn;
    eleNode->entryNum = 1;
    eleNode->nextEle = NULL;

    iter->maxLsn = lsn;

    // If header has one or more element nodes
    if(iter->tailEle != NULL) {
        iter->tailEle->nextEle = eleNode;
        iter->tailEle = eleNode;
    } else { // If no element node linked by this header
        iter->nextEle = eleNode;
        iter->tailEle = eleNode;
    }

//    pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
    pthread_rwlock_unlock(&iter->headLock);
#ifdef ENABLE_DEBUG_INFO
    printf("%s %d\n", __func__ , __LINE__);
    fflush(stdout);
#endif
    return true;
}

// This function will return lsn list that need be replayed
// Parameters:
//      $targetLsn is caller's request lsn, we need to replay until current lsn >= targetLsn
//      $replayedLsn is in this block lsn list, which largest lsn has been replayed, if no page is replayed, set it as 0
//      $toReplayList and $listLen is caller's value, toReplayList should start from first lsn need replay
//      return value: if list not exist, return false. if list exists and no version need to be replayed, return true and an empty list (listLen = 0),
//                      in this case, the replayedLsn is the version that largest lsn that <= targetLsn
// If *listLen > 0, caller should release head's lock and free() *toReplayList
bool HashMapGetBlockReplayList(HashMap hashMap, KeyType key, uint64_t targetLsn, uint64_t *replayedLsn, uint64_t **toReplayList, int *listLen) {
    uint32_t hashValue = HashKey(key);
    uint32_t bucketPos = hashValue % hashMap->bucketNum;

    // Lock this slot
    //    ReaderLock r_lock(hashMap->bucketList[bucketPos].bucketLock);
    pthread_rwlock_rdlock(&hashMap->bucketList[bucketPos].bucketLock);

    // Find the match head
    HashNodeHead* iter = hashMap->bucketList[bucketPos].nodeList;
    bool foundHead = false;
    while(iter != NULL) {

#ifdef ENABLE_DEBUG_INFO
        printf("%s %d \n", __func__ , __LINE__);
        fflush(stdout);
#endif
        // If found matched key, break the loop
        if(iter->hashValue == hashValue
           && KeyMatch(iter->key, key)) {
            foundHead = true;
            break;
        }

        iter = iter->nextHead;
    }

    // TODO, Now we can release bucketList lock

    // If this relation doesn't exist in hash map, return false
    if(!foundHead) {
        pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
        return false;
    }

    // Unlock it until caller func has finished replaying task
    pthread_rwlock_wrlock(&iter->headLock);

    // Check whether we need redo or not
    if (iter->replayedLsn == targetLsn) {
        *listLen = 0;
        *replayedLsn = iter->replayedLsn;
        pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
        pthread_rwlock_unlock(&iter->headLock);
        return true;
    }

    if (iter->replayedLsn > targetLsn) { // no need replay
        // Now, in head, find the largest lsn that <= targetLsn
        if(iter->lsnEntry[iter->entryNum-1].lsn >= targetLsn) { // Should find in head
            uint64_t currentLsn = -1;
            for(int i = 0; i < iter->entryNum; i++) {
                if(iter->lsnEntry[i].lsn > targetLsn) {
                    break;
                } else {
                    currentLsn = iter->lsnEntry[i].lsn;
                }
            }
            // If all list elements are larger than targetLsn, return false
            // This case should not happen
            if(currentLsn == -1) {
                pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
                pthread_rwlock_unlock(&iter->headLock);

                printf("Error, %s can't find any match lsn\n", __func__ );
                return false;
            } else {
                pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
                pthread_rwlock_unlock(&iter->headLock);

                *listLen = 0;
                *replayedLsn = currentLsn;
                return true;
            }

        }

        // Try to find it in following nodes
        // Initialize the currentLsn as the rear lsn of list
        uint64_t  currentLsn = iter->lsnEntry[iter->entryNum-1].lsn;

        HashNodeEle* eleIter = iter->nextEle;
        while(eleIter != NULL) {
            // fast skip
            if(eleIter->maxLsn < targetLsn) {
                currentLsn = eleIter->lsnEntry[eleIter->entryNum-1].lsn;

                eleIter = eleIter->nextEle;
                continue;
            }

            // targetEntry should be found in the list
            for(int i = 0; i < eleIter->entryNum; i++) {
                if(eleIter->lsnEntry[i].lsn > targetLsn) {
                    break;
                } else {
                    currentLsn = eleIter->lsnEntry[i].lsn;
                }
            }

            // Found the desired lsn
            break;
        }
        *replayedLsn = currentLsn;
        *listLen = 0;
        pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
        pthread_rwlock_unlock(&iter->headLock);
        return true;
    }

    // If we have replayed all the page version in this list, and the targetLsn is larger than iter->maxLsn
    if(iter->replayedLsn == iter->maxLsn) {
        *replayedLsn = iter->maxLsn;
        *listLen = 0;
        pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
        pthread_rwlock_unlock(&iter->headLock);
        return true;
    }

    // Now, we should have some versions to be replayed
    *replayedLsn = iter->replayedLsn;
    // Find the where is the last replayed element

    int mallocSize = 16;
    int toReplayCount = 0;
    *toReplayList = (uint64_t*) malloc(sizeof(uint64_t)* mallocSize);

    // last replayed lsn in head
    if(iter->replayedLsn <= iter->lsnEntry[iter->entryNum-1].lsn) {
        for(int i = 0; i < iter->entryNum; i++) {
            if(iter->lsnEntry[i].lsn > iter->replayedLsn
            && iter->lsnEntry[i].lsn <= targetLsn) {

                (*toReplayList)[toReplayCount] = iter->lsnEntry[i].lsn;
                toReplayCount++;
                if(toReplayCount == mallocSize) {
                    mallocSize *= 2;
                    uint64_t* newList = (uint64_t*) realloc(*toReplayList, sizeof(uint64_t)*mallocSize);
                    *toReplayList = newList;
                }
            } else if (iter->lsnEntry[i].lsn > targetLsn) { // found all toReplay list
                break;
            } else { // didn't reach replayLsn
                continue;
            }
        }
    }

    HashNodeEle* eleIter = iter->nextEle;
    while(eleIter != NULL) {
        if(eleIter->lsnEntry[0].lsn > targetLsn) {
            break;
        }

        // fast skip
        if(eleIter->maxLsn < iter->replayedLsn) {
            eleIter = eleIter->nextEle;
            continue;
        }

        // targetEntry should be found in the list
        for(int i = 0; i < eleIter->entryNum; i++) {
            if(eleIter->lsnEntry[i].lsn > iter->replayedLsn
               && eleIter->lsnEntry[i].lsn <= targetLsn) {
                (*toReplayList)[toReplayCount] = eleIter->lsnEntry[i].lsn;
                toReplayCount++;
                if(toReplayCount == mallocSize) {
                    mallocSize *= 2;
                    uint64_t* newList = (uint64_t*) realloc(*toReplayList, sizeof(uint64_t)*mallocSize);
                    *toReplayList = newList;
                }
            }
            else if (eleIter->lsnEntry[i].lsn > targetLsn) { // found all toReplay list
                break;
            } else { // didn't reach replayLsn
                continue;
            }
        }

        eleIter = eleIter->nextEle;
    }

    *listLen = toReplayCount;
    if(toReplayCount == 0) {
        printf("ERROR: %s toReplayCount = 0\n", __func__ );
    }


    pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
    // don't release head's lock
    return true;
}

// [lsn(0), lsn(1), ... lsn(i), lsn(i+1), ..., lsn(n)]
// If lsn(i) <= targetLsn < lsn(i+1), return ( true, lsn(i) )
// If targetLsn >= lsn(n), return true, return ( true, lsn(n) )
// If lsn list is empty, return false
bool HashMapFindLowerBoundEntry(HashMap hashMap, KeyType key, uint64_t targetLsn, uint64_t* foundLsn, int* foundPageNum) {
#ifdef ENABLE_DEBUG_INFO
    printf("%s start, hashMap Address = %p \n", __func__ , hashMap);
    fflush(stdout);
    printf("%s %d bucketNum = %d \n", __func__ , __LINE__, hashMap->bucketNum);
    fflush(stdout);
#endif
    uint32_t hashValue = HashKey(key);
    uint32_t bucketPos = hashValue % hashMap->bucketNum;

#ifdef ENABLE_DEBUG_INFO
    printf("%s start, targetLsn = %lu, hashValue = %u, bucketPos = %u \n", __func__ , targetLsn, hashValue, bucketPos);
    fflush(stdout);
#endif

    // Lock this slot
//    ReaderLock r_lock(hashMap->bucketList[bucketPos].bucketLock);
    pthread_rwlock_rdlock(&hashMap->bucketList[bucketPos].bucketLock);

    // Find the match head
    HashNodeHead* iter = hashMap->bucketList[bucketPos].nodeList;
    bool foundHead = false;
    while(iter != NULL) {

#ifdef ENABLE_DEBUG_INFO
        printf("%s %d \n", __func__ , __LINE__);
        fflush(stdout);
#endif
        // If found matched key, break the loop
        if(iter->hashValue == hashValue
           && KeyMatch(iter->key, key)) {
            foundHead = true;
            break;
        }

        iter = iter->nextHead;
    }

#ifdef ENABLE_DEBUG_INFO
    printf("%s %d \n", __func__ , __LINE__);
    fflush(stdout);
#endif
    // If this relation doesn't exist in hash map, return false
    if(!foundHead) {
        pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
#ifdef ENABLE_DEBUG_INFO
        printf("%s %d \n", __func__ , __LINE__);
        fflush(stdout);
#endif
        return false;
    }

#ifdef ENABLE_DEBUG_INFO
    printf("%s %d \n", __func__ , __LINE__);
    fflush(stdout);
#endif
    // Unlock bucketList, now it's useless
    pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
    // Lock this head
//    ReaderLock r_head_lock(iter->headLock);
    pthread_rwlock_rdlock(&iter->headLock);

    uint64_t currentLsn = -1;
    int      currentPageNum;
    // If lsn is in this head
    // Iterate all the elements in the head
    if(iter->lsnEntry[iter->entryNum-1].lsn >= targetLsn) {
#ifdef ENABLE_DEBUG_INFO
        printf("%s %d \n", __func__ , __LINE__);
        fflush(stdout);
#endif
        for(int i = 0; i < iter->entryNum; i++) {
            if(iter->lsnEntry[i].lsn > targetLsn) {
                break;
            } else {
                currentLsn = iter->lsnEntry[i].lsn;
                currentPageNum = iter->lsnEntry[i].pageNum;
            }
        }
        // If all list elements are larger than targetLsn, return false
        if(currentLsn == -1) {
//            pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
            pthread_rwlock_unlock(&iter->headLock);

#ifdef ENABLE_DEBUG_INFO
            printf("%s %d \n", __func__ , __LINE__);
            fflush(stdout);
#endif
            return false;
        } else {
//            pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
            pthread_rwlock_unlock(&iter->headLock);

            *foundLsn = currentLsn;
            *foundPageNum = currentPageNum;

#ifdef ENABLE_DEBUG_INFO
            printf("%s %d \n", __func__ , __LINE__);
            fflush(stdout);
#endif
            return true;
        }
    }
#ifdef ENABLE_DEBUG_INFO
    printf("%s %d \n", __func__ , __LINE__);
    fflush(stdout);
#endif

    // Iterate all following element nodes

    // Initialize the currentLsn as the rear lsn of list
    currentLsn = iter->lsnEntry[iter->entryNum-1].lsn;
    currentPageNum = iter->lsnEntry[iter->entryNum-1].pageNum;

    HashNodeEle* eleIter = iter->nextEle;
    while(eleIter != NULL) {
#ifdef ENABLE_DEBUG_INFO
        printf("%s %d \n", __func__ , __LINE__);
        fflush(stdout);
#endif
        // fast skip
        if(eleIter->maxLsn < targetLsn) {
            currentLsn = eleIter->lsnEntry[eleIter->entryNum-1].lsn;
            currentPageNum = eleIter->lsnEntry[eleIter->entryNum-1].pageNum;

            eleIter = eleIter->nextEle;
            continue;
        }

        // targetEntry should be found in the list
        for(int i = 0; i < eleIter->entryNum; i++) {
#ifdef ENABLE_DEBUG_INFO
            printf("Get in, comparedLsn = %lu, pageNum = %d\n", eleIter->lsnEntry[i].lsn, eleIter->lsnEntry[i].pageNum);
            fflush(stdout);
#endif
            if(eleIter->lsnEntry[i].lsn > targetLsn) {
                break;
            } else {
                currentLsn = eleIter->lsnEntry[i].lsn;
                currentPageNum = eleIter->lsnEntry[i].pageNum;
            }
        }

        // Found the desired lsn
        break;
    }

#ifdef ENABLE_DEBUG_INFO
    printf("%s %d \n", __func__ , __LINE__);
    fflush(stdout);
#endif
    *foundLsn = currentLsn;
    *foundPageNum = currentPageNum;

    pthread_rwlock_unlock(&iter->headLock);
//    pthread_rwlock_unlock(&hashMap->bucketList[bucketPos].bucketLock);
#ifdef ENABLE_DEBUG_INFO
    printf("%s end\n", __func__ );
    fflush(stdout);
#endif
    return true;
}

//void HashMapUpdateKey(KeyType key, int value) {
//    uint32_t hashValue = HashKey(key);
//    uint32_t bucketPos = hashValue % hashMap->bucketNum;
//    HashBucket* targetBucket = &hashMap->bucketList[bucketPos];
//
//    WriterLock w_lock(targetBucket->bucketLock);
//
//    bool find = false;
//    HashNodeHead* node = targetBucket->nodeList;
//    while(node != NULL) {
//        if(KeyEqual(node->key, key)) {
//            find = true;
//            break;
//        }
//        node = node->next;
//    }
//
//    if(find) {
//        node->value = value;
//        return;
//    } else {
//        HashNodeHead* node = (HashNodeHead*) malloc(sizeof(HashNodeHead));
//        node->key = key;
//        node->bucket = &hashMap->bucketList[bucketPos];
//        node->value = value;
//        node->hashValue = hashValue;
//        node->next = node->bucket->nodeList;
//        node->bucket->nodeList = node;
//        return;
//    }
//}
//
//
//int HashMapFindKey(KeyType key) {
//    uint32_t hashValue = HashKey(key);
//    uint32_t bucketPos = hashValue % hashMap->bucketNum;
//    HashBucket* targetBucket = &hashMap->bucketList[bucketPos];
////    printf("hashValue = %u, Slot = %u\n", hashValue, bucketPos);
//
//    ReaderLock r_lock(targetBucket->bucketLock);
//
//    bool find = false;
//    HashNodeHead* node = targetBucket->nodeList;
//    while(node != NULL) {
//        if(KeyEqual(node->key, key)) {
//            find = true;
//            break;
//        }
//        node = node->next;
//    }
//
//    if(find)
//        return node->value;
//    else
//        return -1; // no find
//}

void HashMapDestroy(HashMap hashMap){
    for(int i = 0; i < hashMap->bucketNum; i++) {
        while (hashMap->bucketList[i].nodeList) {
            HashNodeHead* head = hashMap->bucketList[i].nodeList;

            while(head->nextEle) {
                HashNodeEle* elemNode = head->nextEle;
                head->nextEle = head->nextEle->nextEle;
                free(elemNode);
            }

            hashMap->bucketList[i].nodeList = head->nextHead;
            free(head);
        }
    }
    free(hashMap->bucketList);
    free(hashMap);
}

//void *ReadLock(void*)
//{
//    sleep(1);
//    ReaderLock r_lock(myLock);
//    printf("read_start\n");
//    //Do reader stuff
//    sleep(3);
//    printf("read_end\n");
//    return NULL;
//}
//
//void *WriteLock(void*)
//{
//    WriterLock w_lock(myLock);
//    printf("write_start\n");
//    sleep(5);
//    printf("write_end\n");
//    return NULL;
//}
//
//int GetInteger(int i) {
//    std::cout << "i2nput is " << i << std::endl;
//    return i+1;
//}
