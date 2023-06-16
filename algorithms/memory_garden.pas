program MemoryGarden; { memory_garden.pas }
const
    GardenSize = 64;
    ObjectSize = 4;
    GardenMem = GardenSize * ObjectSize;
type
    MemGarden = record
        mem: array [1..GardenMem] of integer;
        free: integer;
    end;

procedure GardenInit(var garden: MemGarden);
var
    i: integer;
begin
    i := 1;
    while i <= GardenMem do
    begin
        garden.mem[i] := i + ObjectSize;
        if garden.mem[i] > GardenMem then
            garden.mem[i] := 0;
        i := i + ObjectSize
    end;
    garden.free := 1
end;

procedure GardenAllocate(var garden: MemGarden; var p: integer);
begin
    p := garden.free;
    if garden.free <> 0 then
        garden.free := garden.mem[garden.free]
end;

procedure GardenFree(var garden: MemGarden; p: integer);
begin
    garden.mem[p] := garden.free;
    garden.free := p
end;

var
	garden: MemGarden;
    p, n: integer;
begin 
	GardenInit(garden);
	while not SeekEof do
	begin
		readln(n);
        if n = 0 then
        begin
		    GardenAllocate(garden, p);
            writeln('Allocated ', p)
        end
        else
            GardenFree(garden, n)
	end;
	writeln;
    p := garden.free;
	while p <> 0 do
	begin
		writeln(p);
        p := garden.mem[p]
	end
end.

