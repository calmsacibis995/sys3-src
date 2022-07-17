PCC = pcc
CFLAGS = -O
LDFLAGS = -n -s
YACCRM=-rm
IFLAG = -i
V =
INS = :
INSDIR = /lib
CCDIR = /bin
FFLAG =

all:	$Vcpp $Vc0 $Vc1 $Vc2 $Vcc

compiler:	$Vcpp $Vc0 $Vc1 $Vc2

$Vc0::	c00.o c01.o c02.o c03.o c04.o c05.o
	$(CC) $(FFLAG) $(CFLAGS) $(LDFLAGS) -o $Vc0 c00.o c01.o c02.o c03.o c04.o c05.o $(LIBES)
$Vc0::
	$(INS) $Vc0 $(INSDIR)

c00.o c01.o c02.o c03.o c04.o c05.o: c0.h

$Vc1::	c10.o c11.o c12.o c13.o table.o
	$(CC) $(FFLAG) $(CFLAGS) $(LDFLAGS) -o $Vc1 c10.o c11.o c12.o c13.o table.o $(LIBES)
$Vc1::
	$(INS) $Vc1 $(INSDIR)

c10.o c11.o c12.o c13.o: c1.h
table.o: table.s cvopt
	./cvopt <table.s >table.i
	as -o table.o table.i
	rm table.i

$Vc2::	c20.o c21.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(IFLAG) -o $Vc2 c20.o c21.o $(LIBES)
$Vc2::
	$(INS) $Vc2 $(INSDIR)

c20.o c21.o: c2.h
cvopt:	cvopt.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o cvopt cvopt.c $(LIBES)

$Vcpp::	cpp.o cpy.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $Vcpp cpp.o cpy.o $(LIBES)
$Vcpp::
	$(INS) $Vcpp $(INSDIR)

cpp.o: cpp.c
	$(CC) $(CFLAGS) -c cpp.c
cpy.o:	cpy.c yylex.c
	$(CC) $(CFLAGS) -c cpy.c
cpy.c:	cpy.y

$Vcc::	cc.c
	$(CC) -DCSW=0 $(CFLAGS) $(LDFLAGS) -o $Vcc cc.c $(LIBES)
$Vcc::
	$(INS) $Vcc $(CCDIR)

$Vpcc::	cc.c
	$(PCC) -DCSW=1 '-DCCNAME="pcc"' $(CFLAGS) $(LDFLAGS) -o $Vpcc cc.c $(LIBES)
$Vpcc::
	$(INS) $Vpcc $(CCDIR)

install:	all
	make -f cc.mk IFLAG=$(IFLAG) YACCRM=$(YACCRM) all "INS=/etc/install" "V=$V"

backup:
	cp $(INSDIR)/cpp ocpp
	/etc/install ocpp $(INSDIR)
	cp $(INSDIR)/c0 oc0
	/etc/install oc0 $(INSDIR)
	cp $(INSDIR)/c1 oc1
	/etc/install oc1 $(INSDIR)
	cp $(INSDIR)/c2 oc2
	/etc/install oc2 $(INSDIR)
	cp $(CCDIR)/cc occ
	/etc/install occ $(CCDIR)

promote:
	cp $(INSDIR)/ncpp cpp
	/etc/install cpp $(INSDIR)
	cp $(INSDIR)/nc0 c0
	/etc/install c0 $(INSDIR)
	cp $(INSDIR)/nc1 c1
	/etc/install c1 $(INSDIR)
	cp $(INSDIR)/nc2 c2
	/etc/install c2 $(INSDIR)
	cp $(CCDIR)/ncc cc
	/etc/install cc $(CCDIR)

$Vncc::
	make -f cc.mk IFLAG=$(IFLAG) YACCRM=$(YACCRM) "V=n" "INS=$(INS)" ncc
$Vncpp::
	make -f cc.mk IFLAG=$(IFLAG) YACCRM=$(YACCRM) "V=n" "INS=$(INS)" ncpp
$Vnc0::
	make -f cc.mk IFLAG=$(IFLAG) YACCRM=$(YACCRM) "V=n" "INS=$(INS)" nc0
$Vnc1::
	make -f cc.mk IFLAG=$(IFLAG) YACCRM=$(YACCRM) "V=n" "INS=$(INS)" nc1
$Vnc2::
	make -f cc.mk IFLAG=$(IFLAG) YACCRM=$(YACCRM) "V=n" "INS=$(INS)" nc2

$Vocc::
	make -f cc.mk IFLAG=$(IFLAG) YACCRM=$(YACCRM) "V=o" "INS=$(INS)" occ
$Vocpp::
	make -f cc.mk IFLAG=$(IFLAG) YACCRM=$(YACCRM) "V=o" "INS=$(INS)" ocpp
$Voc0::
	make -f cc.mk IFLAG=$(IFLAG) YACCRM=$(YACCRM) "V=o" "INS=$(INS)" oc0
$Voc1::
	make -f cc.mk IFLAG=$(IFLAG) YACCRM=$(YACCRM) "V=o" "INS=$(INS)" oc1
$Voc2::
	make -f cc.mk IFLAG=$(IFLAG) YACCRM=$(YACCRM) "V=o" "INS=$(INS)" oc2

clean:
	-rm -f *.o cvopt
	$(YACCRM) -f cpy.c

clobber:	clean
	-rm -f $Vcpp $Vc0 $Vc1 $Vc2 $Vcc $Vpcc
