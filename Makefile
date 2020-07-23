src = poe.cpp
obj = poe.exe

$(obj) : $(src)
	g++ -static -fpermissive -fexec-charset=GBK -mwindows -o $(obj) $(src)

clean :
	del $(obj)