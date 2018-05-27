all:
	g++ src/picc_temp_i2c.cpp example/example.cpp -I./include -o test
clean:
	rm -rf test
