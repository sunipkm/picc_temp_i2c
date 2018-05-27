all:
	g++ src/picc_temp_i2c.cpp example/example.cpp -I./include -o test -DHDC1010_DEBUG

clean:
	rm -rf test
