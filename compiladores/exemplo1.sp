const TAM = 10;
type vetor = array [15] of integer;
type aluno = record
	nota1 : real;
	nota2 : real
end; 
var A, B, C, D : integer;
var E : vetor;
var F : aluno;
var G : boolean;

begin
	A := TAM + 20;
	B := fatorial(A);
	C := exp(A,B);
	D := media(E);
	F := lerDados();
	if (C <= 10 or (A > B and F.nota1 > 6)) then
	begin
		G := false;
		while G != true do
		begin
			D := D + 1;
			G := D > 6
		end
	end
end
