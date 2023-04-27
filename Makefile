release:
	pio run -e release

debug:
	pio run -e debug

upload:
	pio run -e release -t upload

clean:
	pio run -t clean

db:
	pio run -e release -t compiledb

monitor:
	pio device monitor -b 115200
