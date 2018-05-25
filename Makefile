all:
	g++ src/picc_temp_i2c.cpp example/example.c -I./include -o test
clean:
	rm -rf test