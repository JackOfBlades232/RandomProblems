program PersistentTree; { persistent_tree.pas }
type
	LongNodePos = ^LongNodePtr;
	LongNodePtr = ^LongNode;
	LongNode = record
		data: longint;
		left, right: LongNodePtr;
	end;
	TreeOfLongints = record
		root: LongNodePtr;
	end;

procedure PTreeInit(var tree: TreeOfLongints);
begin
	tree.root := nil
end;

function SearchTree(node: LongNodePtr; data: longint): boolean; forward;
function PTreeSearch(var tree: TreeOfLongints; data: longint): boolean;
begin
    PTreeSearch := SearchTree(tree.root, data)
end;

function SearchTree(node: LongNodePtr; data: longint): boolean;
begin
    if node = nil then
        SearchTree := false
    else if node^.data = data then
        SearchTree := true
    else
        SearchTree := SearchTree(node^.left, data) or 
                      SearchTree(node^.right, data)
end;

procedure PTreeInsert(var tree, NewTree: TreeOfLongints;
    data: longint; var ok: boolean);
var
    node: LongNodePtr;
    NewPos: LongNodePos;
begin
    ok := false;
    if SearchTree(tree.root, data) then
        exit;
    PTreeInit(NewTree);
    node := tree.root;
    NewPos := @NewTree.root;

    write('(Re)Created elements: ');

    while node <> nil do
    begin
        new(NewPos^);
        NewPos^^.data := node^.data;

        write(node^.data, ' ');

        if data < node^.data then
        begin
            NewPos^^.right := node^.right;
            NewPos := @NewPos^^.left;
            node := node^.left
        end
        else
        begin
            NewPos^^.left := node^.left;
            NewPos := @NewPos^^.right;
            node := node^.right
        end
    end;

    writeln(data);

    new(NewPos^);
    NewPos^^.data := data;
    NewPos^^.left := nil;
    NewPos^^.right := nil;
    ok := true
end;

procedure PrintTree(var node: LongNodePtr; depth: integer); forward;
procedure PTreePrint(var tree: TreeOfLongints);
begin
    PrintTree(tree.root, 0)
end;

procedure PrintTree(var node: LongNodePtr; depth: integer);
var
    i: integer;
begin
    if node = nil then
        exit;
    PrintTree(node^.left, depth+1);
    for i := 1 to depth do
        write('    ');
    writeln(node^.data);
    PrintTree(node^.right, depth+1)
end;

var
    TreeVersions: array [1..32] of TreeOfLongints;
    VersionIdx: integer = 1;
    n, i: integer;
    ok: boolean = true;
begin
    PTreeInit(TreeVersions[VersionIdx]);
    write('Input set members: ');
    while (not SeekEoln) and ok and (VersionIdx < 32) do
    begin
        read(n);
        PTreeInsert(TreeVersions[VersionIdx], 
            TreeVersions[VersionIdx+1], n, ok);
        VersionIdx := VersionIdx + 1
    end;
    if (not ok) or (VersionIdx = 32) then
    begin
        writeln(ErrOutput, 'Addition failed (or reached version limit)');
        halt(1)
    end;
    writeln('Set versions:');
    writeln;
    for i := 1 to VersionIdx do
    begin
        writeln('Version ', i, ':');
        PTreePrint(TreeVersions[i]);
        writeln
    end
    { No freeing, since it is a non-trivial and a separate problem
      for such trees }
end.

