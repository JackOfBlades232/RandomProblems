program CLinkedList; { cyclic_linked_list.pas }
type
	LongItemPtr = ^LongItem;
	LongItem = record
		data: longint;
		next: LongItemPtr;
	end;
	CListOfLongints = record
		first: LongItemPtr;
	end;

procedure CListInit(var list: CListOfLongints);
begin
	list.first := nil
end;

procedure CListInsert(var list: CListOfLongints; n: longint);
var
    tmp: LongItemPtr;
begin
	if list.first = nil then
	begin
		new(list.first);
        list.first^.next := list.first
	end
	else
	begin
		new(tmp);
        tmp^.next := list.first^.next;
        list.first^.next := tmp
	end;
    list.first^.next^.data := n
end;

function CListSearch(var list: CListOfLongints; n: longint): LongItemPtr;
var
	tmp: LongItemPtr;
begin
    if list.first^.data = n then
    begin
        CListSearch := list.first;
        exit
    end;
    CListSearch := nil;
    tmp := list.first^.next;
    while tmp <> list.first do
    begin
        if tmp^.data = n then
        begin
            CListSearch := tmp;
            break
        end;
        tmp := tmp^.next
    end
end;

procedure CListDelete(var list: CListOfLongints; item: LongItemPtr);
var
    prev: LongItemPtr;
begin
    prev := list.first;
    while prev^.next <> item do
        prev := prev^.next;
    if prev = item then
        list.first := nil
    else
    begin
        prev^.next := item^.next;
        if item = list.first then
            list.first := prev
    end;
    dispose(item)
end;

function CListIsEmpty(var list: CListOfLongints): boolean;
begin
	CListIsEmpty := list.first = nil
end;

var
	l1: CListOfLongints;
    item: LongItemPtr;
	n: longint;
begin { double number problem through list interface }
	CListInit(l1);
	while not SeekEof do
	begin
		read(n);
		CListInsert(l1, n);
	end;
    writeln;
    while not CListIsEmpty(l1) do
	begin
        n := l1.first^.data;
        item := CListSearch(l1, n);
        if item <> nil then
        begin
            CListDelete(l1, item);
            writeln(n)
        end
	end;
end.
