CC = gcc
CXX = g++
SRCS := $(wildcard *.c *.cpp)
EXES := $(SRCS:%.c=%.exe) $(SRCS:%.cpp=%.exe)

all: $(EXES)
	
%.exe: %.cpp
	$(CXX) -o $@ $<

searchingSID: searchingSID.cpp	
	$(CXX) -o $@.exe $< -ladvapi32 

credentialPrompt: credentialPrompt.cpp	
	$(CXX) -o $@.exe $< -lcredui

CreateProcessWithLogonW: CreateProcessWithLogonW.cpp
	$(CXX) -o $@.exe $< -luserenv
	
clean:
	del $(wildcard *.exe)
