CXX ?= g++
CXXFLAGS ?= -std=c++20 -Wall -Wextra -pedantic
PROGRAM := bacm_system
SOURCES = Account.cpp AccountManager.cpp Address.cpp CompanyBST.cpp Contact.cpp CsvUtils.cpp DateTimeUtils.cpp InputUtils.cpp StateValidator.cpp BACM\ System.cpp

.PHONY: all clean run

all:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(PROGRAM)

run: all
	./$(PROGRAM)

clean:
	rm -f $(PROGRAM) *.exe *.o *.obj *.pdb *.ilk *.idb
