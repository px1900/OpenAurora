/*-------------------------------------------------------------------------
 *
 * md.h
 *	  magnetic disk storage manager public interface declarations.
 *
 *
 * Portions Copyright (c) 1996-2020, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/storage/md.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef MD_H
#define MD_H

#include "storage/block.h"
#include "storage/relfilenode.h"
#include "storage/smgr.h"
#include "storage/sync.h"
#include "storage/fd.h"

typedef struct _MdfdVec
{
	File		mdfd_vfd;		/* fd number in fd.c's pool */
	BlockNumber mdfd_segno;		/* segment number, from 0 */
} MdfdVec;

#ifdef __cplusplus
extern "C" {
#endif

/* md storage manager functionality */
extern void mdinit(void);
extern void mdopen(SMgrRelation reln);
extern void mdclose(SMgrRelation reln, ForkNumber forknum);
extern void mdcreate(SMgrRelation reln, ForkNumber forknum, bool isRedo);
extern bool mdexists(SMgrRelation reln, ForkNumber forknum);
extern void mdunlink(RelFileNodeBackend rnode, ForkNumber forknum, bool isRedo);
extern void mdextend(SMgrRelation reln, ForkNumber forknum,
					 BlockNumber blocknum, char *buffer, bool skipFsync);
extern bool mdprefetch(SMgrRelation reln, ForkNumber forknum,
					   BlockNumber blocknum);
extern void mdread(SMgrRelation reln, ForkNumber forknum, BlockNumber blocknum,
				   char *buffer);
extern void mdwrite(SMgrRelation reln, ForkNumber forknum,
					BlockNumber blocknum, char *buffer, bool skipFsync);
extern void mdwriteback(SMgrRelation reln, ForkNumber forknum,
						BlockNumber blocknum, BlockNumber nblocks);
extern BlockNumber mdnblocks(SMgrRelation reln, ForkNumber forknum);
extern void mdtruncate(SMgrRelation reln, ForkNumber forknum,
					   BlockNumber nblocks);
extern void mdimmedsync(SMgrRelation reln, ForkNumber forknum);

extern void ForgetDatabaseSyncRequests(Oid dbid);
extern void DropRelationFiles(RelFileNode *delrels, int ndelrels, bool isRedo);

/* md sync callbacks */
extern int	mdsyncfiletag(const FileTag *ftag, char *path);
extern int	mdunlinkfiletag(const FileTag *ftag, char *path);
extern bool mdfiletagmatches(const FileTag *ftag, const FileTag *candidate);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* local routines */
void mdunlinkfork(RelFileNodeBackend rnode, ForkNumber forkNum,
						 bool isRedo);
MdfdVec *mdopenfork(SMgrRelation reln, ForkNumber forknum, int behavior);
void register_dirty_segment(SMgrRelation reln, ForkNumber forknum,
								   MdfdVec *seg);
void register_unlink_segment(RelFileNodeBackend rnode, ForkNumber forknum,
									BlockNumber segno);
void register_forget_request(RelFileNodeBackend rnode, ForkNumber forknum,
									BlockNumber segno);
void _fdvec_resize(SMgrRelation reln,
						  ForkNumber forknum,
						  int nseg);
char *_mdfd_segpath(SMgrRelation reln, ForkNumber forknum,
						   BlockNumber segno);
MdfdVec *_mdfd_openseg(SMgrRelation reln, ForkNumber forkno,
							  BlockNumber segno, int oflags);
MdfdVec *_mdfd_getseg(SMgrRelation reln, ForkNumber forkno,
							 BlockNumber blkno, bool skipFsync, int behavior);
BlockNumber _mdnblocks(SMgrRelation reln, ForkNumber forknum,
							  MdfdVec *seg);

#ifdef __cplusplus
}
#endif

#endif							/* MD_H */
