
---

## ✅ `Makefile`

```makefile
CXX = g++
CXXFLAGS = -Ilib -Wall -Wextra
SRC = main.cpp lib/myserial.cpp lib/myplc.cpp lib/ModbusRTUSlave.cpp lib/ton.cpp
OUT = build/programma.exe

all: $(OUT)

$(OUT): $(SRC)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -rf build/
