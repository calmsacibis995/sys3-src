all:	machine system drivers pwbstuff

machine:
	cd ml; make -f ml.mk "FRC=$(FRC)" "INCRT=$(INCRT)"

system:
	cd os; make -f os.mk "FRC=$(FRC)" "INCRT=$(INCRT)"

drivers:
	cd io; make -f io.mk "FRC=$(FRC)" "INCRT=$(INCRT)"

pwbstuff:
	cd pwb; make -f pwb.mk "FRC=$(FRC)" "INCRT=$(INCRT)"

clean:
	cd ml; make -f ml.mk clean
	cd os; make -f os.mk clean
	cd io; make -f io.mk clean
	cd pwb; make -f pwb.mk clean

clobber:
	cd ml; make -f ml.mk clobber
	cd os; make -f os.mk clobber
	cd io; make -f io.mk clobber
	cd pwb; make -f pwb.mk clobber

FRC:
