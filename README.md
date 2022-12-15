# GarbageCollector

**개발 취지:** 쓰레기통에 IOT를 접목시켜 소형 쓰레기 처리의 불편함을 제거하기 위함.

**기능 목록:**
1. 잔여 용량 확인: 쓰레기통의 남은 용량을 실외에서도 멀리서도 확인할 수 있도록 함.
2. 잔여 배터리 확인: 쓰레기통의 전력 공급 상태를 확인할 수 있도록 함.
3. 원격 조종: 모바일 어플리케이션(GarbageCollectorManager; GCM)을 사용하여 쓰레기통 위치를 변경할 수 있도록 함.

**사용 모듈:**
- Arduino Mega 2560 1개
- SG-90 2개 (서보 모터) 
- SR-04 2개 (초음파 센서)
- DC모터 2개
- LCD2004A 1개
- HC-05 1개 (블루투스 모듈)
- 브레드보드 1개

**주의:**
- MyBluetooth 폴더를 'C:\Users\UserName\Documents\Arduino\libraries' 경로에 넣어야합니다.
