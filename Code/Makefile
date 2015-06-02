BBBIOLIB_PATH = Class/BBBIOlib/BBBio_lib
EIGEN_PATH = /usr/include/eigen3/

all: balance

#
# Classes
#
IMU.o: Class/IMU.cpp Class/IMU.hpp Data3D.o
	g++ -I ${EIGEN_PATH} -c Class/IMU.cpp -Wall

Data3D.o: Class/Data3D.cpp Class/Data3D.hpp
	g++ -I ${EIGEN_PATH} -c Class/Data3D.cpp -Wall

MotorDriver.o: Class/MotorDriver.cpp Class/MotorDriver.hpp BBBiolib.o
	g++ -c Class/MotorDriver.cpp -Wall -std=c++11

ADC.o: Class/ADC.cpp Class/ADC.hpp Class/constants.hpp BBBiolib.o
	g++ -c Class/ADC.cpp -Wall -std=c++11

eQEP.o: Class/eQEP.cpp Class/eQEP.hpp
	g++ -c Class/eQEP.cpp -Wall -std=c++11

PID.o: Class/PID.cpp Class/PID.hpp
	g++ -c Class/PID.cpp -Wall

BBBiolib_McSPI.o: ${BBBIOLIB_PATH}/BBBiolib_McSPI.c ${BBBIOLIB_PATH}/BBBiolib_PWMSS.h
	gcc -c ${BBBIOLIB_PATH}/BBBiolib_McSPI.c -Wall -Wno-unused-variable -Wno-unused-but-set-variable

BBBiolib_ADCTSC.o: ${BBBIOLIB_PATH}/BBBiolib_ADCTSC.c ${BBBIOLIB_PATH}/BBBiolib_ADCTSC.h
	gcc -c ${BBBIOLIB_PATH}/BBBiolib_ADCTSC.c -Wall -Wno-unused-variable -Wno-unused-but-set-variable

BBBiolib_PWMSS.o: ${BBBIOLIB_PATH}/BBBiolib_PWMSS.c ${BBBIOLIB_PATH}/BBBiolib_PWMSS.h
	gcc -c ${BBBIOLIB_PATH}/BBBiolib_PWMSS.c -Wall -Wno-unused-variable -Wno-unused-but-set-variable

BBBiolib.o: ${BBBIOLIB_PATH}/BBBiolib.c ${BBBIOLIB_PATH}/BBBiolib.h BBBiolib_McSPI.o BBBiolib_ADCTSC.o BBBiolib_PWMSS.o
	gcc -c ${BBBIOLIB_PATH}//BBBiolib.c -Wall -Wno-unused-variable -Wno-unused-but-set-variable -Wno-pointer-sign -Wno-char-subscripts

Segway.o: Class/Segway.cpp Class/Segway.hpp Class/constants.hpp IMU.o Data3D.o BBBiolib.o MotorDriver.o ADC.o eQEP.o
	g++ -I ${EIGEN_PATH} -c Class/Segway.cpp -Wall -Wno-deprecated -std=c++11

#
# Drivers
#
#main.o: main.cpp IMU.hpp MotorDriver.hpp ADC.o
#	g++ -I ${EIGEN_PATH} -c main.cpp -Wall -std=c++11

main-IMU.o: Data3D.o IMU.o Class/Test/main-IMU.cpp
	g++ -I ${EIGEN_PATH} -c Class/Test/main-IMU.cpp -Wall -std=c++11

main-Motor.o: MotorDriver.o BBBiolib.o Class/main-Motor.cpp
	g++ -I ${EIGEN_PATH} -c Class/Test/main-Motor.cpp -Wall -std=c++11

main-ADC.o: ADC.o BBBiolib.o Class/main-ADC.cpp
	g++ -c Class/Test/main-ADC.cpp -Wall -std=c++11

main-eQEP.o: eQEP.o MotorDriver.o Class/main-eQEP.cpp
	g++ -c Class/Test/main-eQEP.cpp -Wall	-std=c++11

balance.o: balance.cpp IMU.o Data3D.o BBBiolib.o MotorDriver.o Segway.o ADC.o eQEP.o PID.o
	g++ -I ${EIGEN_PATH} -c balance.cpp -Wall -Wno-deprecated -std=c++11

#
# Class drivers
#
main-IMU: main-IMU.o
	g++ main-IMU.o IMU.o Data3D.o -o main-IMU

main-Motor: main-Motor.o
	g++ main-Motor.o MotorDriver.o BBBiolib_PWMSS.o BBBiolib.o BBBiolib_ADCTSC.o BBBiolib_McSPI.o -o main-Motor

main-ADC: main-ADC.o
	g++ main-ADC.o ADC.o BBBiolib_PWMSS.o BBBiolib.o BBBiolib_ADCTSC.o BBBiolib_McSPI.o -o main-ADC	

main-eQEP: main-eQEP.o
	g++ main-eQEP.o eQEP.o MotorDriver.o BBBiolib_PWMSS.o BBBiolib.o BBBiolib_ADCTSC.o BBBiolib_McSPI.o -o main-eQEP

balance: balance.o
	g++ -I /usr/include/eigen3/ balance.o IMU.o Data3D.o MotorDriver.o ADC.o eQEP.o PID.o Segway.o BBBiolib_PWMSS.o BBBiolib_McSPI.o BBBiolib_ADCTSC.o BBBiolib.o -o balance

#
# Clean
#
.PHONY: clean
clean:
	rm -r *.o main-* balance