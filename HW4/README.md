#### fifo를 이용한 client-server 프로그램

##### server
- 최대 3명의 client와 통신하는 server 프로그램
- client의 접속이 없는 경우 작업대기를 한다.
- 접속된 client의 수가 1명 또는 2명인 경우에도 작업을 한다.
- client가 보낸 정수 값에 +8을 한 후 해당 client에게 다시 보낸다.
- client로 부터의 정수 전달 없이 60초가 지속되면 server는 종료한다.

##### client
- server에 접속하기 위해 자신의 id를 할당받는다.
- 현재 통신에 참여하고 있는 client가 3명인 경우, id 할당 과정에서 대기한다.
- 한 번 종료한 client도 다시 id를 할당받고 실행을 시작할 수 있지만, 이전에 사용했던 id로의 재접속을 보장하지 않는다.
