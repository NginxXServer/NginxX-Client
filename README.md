
# NginxX Client - Siege 테스트 자동화

이 프로젝트는 `siege`를 이용해 서버 성능 테스트를 자동화하고, 결과를 `log` 디렉토리에 저장하는 프로그램을 포함하고 있습니다. 

## 디렉토리 구조

프로젝트의 주요 디렉토리와 파일은 다음과 같습니다:

```
src
├── test                # 테스트를 위한 디렉토리
│   ├── Makefile        # 컴파일을 위한 Makefile
│   ├── log             # 테스트 결과가 저장되는 로그 디렉토리
│   ├── main.c          # Siege 테스트를 실행하는 메인 프로그램
│   ├── siege.conf      # Siege 설정 파일
│   └── urls.txt        # 테스트할 URL 목록
├── version1            # 이전 버전 (사용하지 않음)
└── version2            # 이전 버전 (사용하지 않음)
```

## 사전 준비 사항

1. **siege.conf 설정**  
   `siege.conf` 파일에는 다음 설정이 포함되어 있습니다. 필요에 따라 수정 가능합니다:
   ```
   limit=10000
   # concurrent=1000
   protocol=HTTP/1.1
   # connection=close
   ```

2. **URL 목록 준비**  
   `urls.txt` 파일에는 7개의 URL이 포함되어 있습니다. URL은 자유롭게 추가하거나 수정할 수 있습니다. 예시:
   ```
   http://10.198.138.213:39071/?docName=a          # 1KB 문서
   http://10.198.138.213:39071/?docName=b          # 1KB 문서
   http://10.198.138.213:39071/?docName=c          # 1KB 문서
   http://10.198.138.213:39071/?docName=940.jpg    # 940KB 이미지
   http://10.198.138.213:39071/?docName=imgae.png  # 317KB 이미지
   http://10.198.138.213:39071/?docName=index.html # 1KB 문서
   http://10.198.138.213:39071/?docName=ttttttt    # 존재하지 않는 문서 (404 에러)
   ```

3. **로그 디렉토리**  
   `log` 디렉토리는 이미 존재한다고 가정합니다. 해당 디렉토리에 테스트 결과가 저장됩니다.

---

## 테스트 프로그램 컴파일 및 실행 방법

1. **컴파일**  
   `src/test` 디렉토리로 이동한 뒤, 아래 명령어로 프로그램을 컴파일합니다:
   ```bash
   make
   ```

2. **프로그램 실행**  
   컴파일 후 생성된 `run_siege` 파일을 실행합니다:
   ```bash
   ./run_siege
   ```

---

## 사용자 입력 및 출력

1. **입력**  
   프로그램 실행 시 아래 입력을 요구합니다:
   - **동시 접속자 수**: 테스트에 사용할 동시 접속자 수 (예: 1000).
   - **테스트 실행 시간**: 테스트를 실행할 시간 (예: `3m`은 3분, `30s`는 30초).
   - **로그 파일 이름**: 테스트 결과를 저장할 로그 파일의 이름 (예: `test_log`).

2. **출력**  
   실행 결과는 다음과 같은 형식으로 출력됩니다. 또한, 결과는 지정된 로그 파일에 저장됩니다:
   ```
   ** SIEGE 4.0.7
   ** Preparing 1000 concurrent users for battle.
   The server is now under siege...
   Lifting the server siege...
   Transactions:		       14638 hits
   Availability:		      100.00 %
   Elapsed time:		      182.53 secs
   Data transferred:	     1870.26 MB
   Response time:		       12.34 secs
   Transaction rate:	       80.20 trans/sec
   Throughput:		       10.25 MB/sec
   Concurrency:		      989.23
   Successful transactions:       10803
   Failed transactions:	           0
   Longest transaction:	       76.21
   Shortest transaction:	        0.17
   ```

---

### 실행 예제

**프로그램 실행**:
```bash
./run_siege
```

**사용자 입력**:
```
Enter the number of connections: 1000
Enter the duration (e.g., 3m, 5s): 3m
Enter the log file name (without extension): performance_test
```

**출력 결과**:
```
** SIEGE 4.0.7
** Preparing 1000 concurrent users for battle.
The server is now under siege...
Lifting the server siege...
Transactions:		       14638 hits
Availability:		      100.00 %
Elapsed time:		      182.53 secs
Data transferred:	     1870.26 MB
Response time:		       12.34 secs
Transaction rate:	       80.20 trans/sec
Throughput:		       10.25 MB/sec
Concurrency:		      989.23
Successful transactions:       10803
Failed transactions:	           0
Longest transaction:	       76.21
Shortest transaction:	        0.17
```
