# Disaggregated DBMS

Storage-compute disaggregation has recently emerged as a novel architecture in modern data centers, particularly in the cloud. However, little is known about the effectiveness of the design choices in these databases since they are typically developed by industry giants, and only the overall performance results are presented without detailing the impact of individual design principles.

In this project, we  implemented  **five** different storage-compute disaggregation architecutres based on PostgreSQL. We aim to investigate the performance implications of the design choices that widely adopted in storage-disaggregated databases.

# Other Architectures' Link

This is **Remote Disk**'s source code repository.
You can find other architecture's source code from the following links:

Monolithic: [PostgreSQL v13.0 ](https://www.postgresql.org/ftp/source/v13.0/)

LogDB: [Link for LogDB](https://anonymous.4open.science/r/Disaggregated-DBMS-LogDB)

LogDB-MV: [Link for LogDB-MV](https://anonymous.4open.science/r/Disaggregated-DBMS-LogDB-MV)

LogDB-MV-FR: [Link for LogDB-MV-FR](https://anonymous.4open.science/r/Disaggregated-DBMS-LogDB-MV-FR)

LogDB-MV-SR: [Link for LogDB-MV-SR](https://anonymous.4open.science/r/Disaggregated-DBMS)


# Architecture Overview

<img src="pictures/E2_ARCH_NEW.png" width="500">




# Implementation 
All the implementations are based on PostgreSQL v13.0. To ensure full compatibility with the PostgreSQL source code, we use C to implement kernel parts, such as xlog replaying and the multi-version storage engine. For network communication, we adopt Apache Thrift v0.14.0~\cite{Thrift}, which is a lightweight software stack for RPC implementation. To achieve compatibility with Apache Thrift, APIs that are closely related to RPC are implemented in C++.

## New code developed based on PostgreSQL

Here are our implemented codes that closely related with disaggregation. Note that there are many files that implemented by this project but are not included in this list. For details, please refer to our code.

```sh
LogDB-MV-SR
├── README.md                  # Project README file
├── include                    # Root folder for header files
│   ├── access                 # Folder for storage access layer 
│   ├── storage                # Folder for storage engine layer
│   ├── tcop                   # Folder for standalone process layer
│   ...
├── backend                    # Root folder for c source code
│   ├── tramsam                # Folder for transaction related functions 
│   │   ├── xlog.c             # Logics for read and write xlogs to storage nodes
│   │   ├── xlog_insert.c      # Control how to assemble xlog 
│   │   ├── timeline.c         # Read timeline from remote stoarge node
│   │   ...
│   ├── storage                # Folder for storage interaction related functions
│   │   ├── rpc                # SubFolder for rpc logic
│   │   │   ├── rpcclient.cpp  # RPC client interfaces
│   │   │   ├── rpcserver.cpp  # GetPage@LSN logic and RPC server interfaces
│   │   │   ├── tutorial.thrift# Thrift definition for RPC service
│   │   ├── buffer             # SubFolder for buffer management
│   │   │   ├── bufmgr.c       # New RPC Buffer Eviction Protocol
│   │   │   ...
│   │   ├── file               # SubFolder for file access
│   │   │   ├── fd.c           # Multi-thread safe file access functions
│   │   │   ...
│   │   ├── smgr               # Folder for storage manager
│   │   │   ├── smgr.c         # Storage manager logic
│   │   │   ├── md.c           # Relation file access layer - md
│   │   │   ├── rpcmd.c        # Compute node's relation file access layer - rpcmd
│   │   │   ...
│   │   ...
│   ├── replication            # Folder for replica functions
│   │   ├── walreceiver.c      # Service for receiving xlogs
│   │   ├── walsender.c        # Service for sending xlogs
│   │   ...
│   ├── tcop                   # Folder for standalone process layer
│   │   ├── storage_service.c  # Storage node's startup process and communication logic
...

```

## Some important configuration locations
* **Storage node's ip**: Storage node's ip should be decleard in ***/backend/access/storage/rpc/rpcclient.c*** "PRIMARY_NODE_IP" MACRO.
* **RPC server**: You can choose RPC server model and configurations. Currently, storage node RPC is using thread-pool model with default 50 pool size. You can update it in ***/backend/access/storage/rpc/rpcserver.c***.
* **multi-threads safe service**: PostgreSQL is a multi-process service. To accomodate multi-thread environment, we updated some original logic to multi-threads safe, for extar -zxvf postgresqlample, file access logic (***/backend/access/storage/file/fd.c***).  You can disable these feature using the bulit-in MACRO

# Before Installment
## PostgreSQL Source Code: [PostgreSQLv13.0](https://www.postgresql.org/ftp/source/v13.0/)
## Core Library
* Boost v1.81.0: [boostorg/boost](https://github.com/boostorg/boost)
* Apache Thrift v0.14.0: [apache/thrift](https://github.com/apache/thrift)

## Other Necessary Library
> byacc, bison, flex, libssl, libsnappy, libflags, libbz2, libz4, libzstd, readline-dev

# Quick Start
## 1. Download [PostgreSQL v13.0](https://www.postgresql.org/ftp/source/v13.0/) source code.
## 2. Uncompress PostgreSQL source code 
```
 tar -zxvf postgresql-13.0.tar.gz
```
## 3. Download this project to PostgreSQL's source code
```
cd postgresql-13.0/src/
git init
git remote origin "$this_repo_address" # temporary delete because of SIGMOD anonymous policy
git fetch; git reset --hard origin/"$branch_name" # temporary delete because of SIGMOD anonymous policy
```
## 4. Compile and install this project
```
cd postgresql-13.0
# using --prefix to declare your target install directory
./configure --prefix=$YOUR_INSTALL_LOCATION LDFLAGS='-std=c++17 -lstdc++ -lrocksdb -lthrift -lrt -ldl -lsnappy -lgflags -lz -lbz2 -llz4 -lzstd -lrocksdb  -lpthread -I. -I/usr/local/include -I/usr/include -L/usr/local/lib -L/usr/bin'
make
make install
```
## 5. Initialize a database for storage node
```
cd $YOUR_INSTALL_LOCATION/bin
./initdb -D $YOUR_DATABASE_LOCATION
```
## 6. Initialize a database for compute node
```
# You can simply use scp or use initdb_comp tool provided by this project
scp $YOUR_DATABASE_LOCATION $YOUR_COMPUTE_DATABASE_LOCATION
```
## 7. Start storage node
```
# Demontrate your database location
export PGDATA="$YOUR_DATABASE_LOCATION" 
./$YOUR_INSTALL_LOCATION/bin/postgres --rpc-server
```
## 8. Start compute node
```
export RPC_CLIENT=1
./$YOUR_INSTALL_LOCATION/bin/pg_ctl -D $YOUR_COMPUTE_DATABASE_LOCATION
```
## 9. Try this disaggregated database
```
./$YOUR_INSTALL_LOCATION/bin/psql postgres
> postgres=# 
```
# Performance 



**Performance Comparison with Monolithic on 100GB SysBench data set**


 <img src="pictures/A2_PERF_1" width="600">


**Performance Comparison with Monolithic on 100GB SysBench data set**


 <img src="pictures/A2_PERF_2" width="600">


**Networking Data Size with Light Workload**


 <img src="pictures/A2_PERF_3" width="500">


**Networking Data Size with Heavy Workload**


 <img src="pictures/A2_PERF_4" width="500">





