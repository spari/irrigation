
# Board 
#FQBN=arduino:avr:diecimila
#FQBN=esp8266:esp8266:d1_mini
FQBN=esp8266:esp8266:nodemcuv2

PORT=/dev/ttyUSB0

all: build upload

build:
	arduino-cli compile --fqbn ${FQBN}

upload:
	arduino-cli upload -p ${PORT} --fqbn ${FQBN}

clean:
	rm *.bin *.elf	

