# Zinny

zinny 는 trigger 엔진에서 사용되는 패키지 매니저 입니다.

패키징시 결과물로는 옵션에 따라 달라지며 공통적으로 `.pot` 파일을 만들어 냅니다.

## Todo

- [ ] init 			
- [ ] build
- [x] help
- [ ] pack -> -p {out name} {filenames...}
- [x] unpack -> -u {filename.pot} {path}
- [x] view -> -v {filename.pot}
- [ ] all check -> -a
- [x] check -> -c {file}

## Command

### init 

현재 위치에 trigger 엔진이 작업할 수 있도록 초기화 합니다.



### build

현재 위치의 `build.toml `을 읽어 빌드를 진행 합니다.



### pack

-p 로 줄일 수 있으며 `{out name} `의 `out_name.pot` 파일을 만들어 냅니다.

해당 파일에 들어가야 하는 파일들은 `{filenames...}` 로 정할 수 있습니다.

`--ur` 을 붙이면 해당 pot 의 내부를 조회할 수 없도록 합니다.



### unpack 

-u 로 줄일 수 있으며 `filename.pot` 을 `{path}` 에 해제 합니다.

만약  `{path}` 를 입력하지 않을 시 기본 경로 `filename/` 에 해제 합니다.



### view

-v 로 줄일 수 있으며 `{filename.pot}` 의 내부를 보여줍니다.



### all check 

-a 로 줄일 수 있으며 해당 경로의 trigger 엔진을 빌드가 가능한지 확인해 줍니다. 

확인은 아래의 순서를 따릅니다.

1. build.toml 을 읽어 버전과 내부 명세를 확인 합니다.
2. 해당 명세를 따라 pot 들을 조회 합니다. 

만약 문제가 있다면 무엇이 문제인지 출력하고 문제가 없다면 종료 합니다.



### check

-c 로 줄일 수 있으며 {file} 이 올바른 pot 인지 확인합니다.

