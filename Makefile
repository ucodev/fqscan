all:
	cd librfsp/ && ./do
	cd libfqscan/ && ./do
	cd fqscan/ && ./do
	cd fqplot/ && ./do
	cd tools/ && ./do
	cd www/ && ./do
	cd dbms/ && ./do

clean:
	cd librfsp/ && ./undo
	cd libfqscan/ && ./undo
	cd fqscan/ && ./undo
	cd fqplot/ && ./undo
	cd tools/ && ./undo
	cd www/ && ./undo
	cd dbms/ && ./undo

