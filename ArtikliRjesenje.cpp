#include <iostream>
#include <iomanip>

using namespace std;

const char * crt = "\n--------------------------------\n";

class KupljeniArtikal {
	char * _naziv;
	float _cijena;
	int _kolicina;
	//konstruktor, destruktor i potrebne funkcije
public:

	KupljeniArtikal(): _naziv(nullptr), _cijena(0), _kolicina(0) {

	}

	KupljeniArtikal(const char * naziv, float cijena) : _cijena(cijena), _kolicina(1) {
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);
	}

	~KupljeniArtikal() {
		delete[] _naziv; _naziv = nullptr;
	}

	KupljeniArtikal& operator+=(float cijena) {
		_cijena += cijena;

		return *this;
	}

	KupljeniArtikal& operator++() {
		++_kolicina;

		return *this;
	}

	KupljeniArtikal& operator=(const KupljeniArtikal& a) {
		//Sprijeciti samododjelu
		if (this != &a) {
			delete[] _naziv;
			int vel = strlen(a._naziv) + 1;
			_naziv = new char[vel];
			strcpy_s(_naziv, vel, a._naziv);

			_cijena = a._cijena;
			_kolicina = a._kolicina;
		}

		return *this;
	}

	friend bool operator==(const KupljeniArtikal&, const KupljeniArtikal&);

	int GetKolicina() const {
		return _kolicina;
	}

	float GetCijena() const {
		return _cijena;
	}

	void UvecajKolicinu(int kol) {
		_kolicina += kol;
	}

	friend ostream& operator<<(ostream&, const KupljeniArtikal&);
};

ostream& operator<<(ostream& cout, const KupljeniArtikal& a) {
	cout << a._naziv << "\t" << a._kolicina << "\t" << a._cijena << endl;
	return cout;
}

bool operator==(const KupljeniArtikal& a1, const KupljeniArtikal& a2) {
	return strcmp(a1._naziv, a2._naziv) == 0;
}





class Racun {
	static int _brojacRacuna;
	const int _brojRacuna; //brojevi racuna krecu od 1
	KupljeniArtikal * _artikli; //niz povecavamo prilikom svakog dodavanja artikla
	int _trenutno;
public:

	Racun(): _brojRacuna(++_brojacRacuna), _artikli(nullptr), _trenutno(0) {
	}

	~Racun() {
		delete[] _artikli; _artikli = nullptr;
	}

	Racun& operator+=(const KupljeniArtikal& artikal) {

		for (int i = 0; i < _trenutno; i++)
		{
			if (_artikli[i] == artikal) {
				_artikli[i].UvecajKolicinu(artikal.GetKolicina());

				return *this;
			}
		}

		KupljeniArtikal* temp = new KupljeniArtikal[_trenutno + 1];
		for (int i = 0; i < _trenutno; i++)
		{
			temp[i] = _artikli[i]; //operator dodjele za KupljeniArtikal
		}

		delete[] _artikli;
		_artikli = temp;
		_artikli[_trenutno] = artikal;
		_trenutno++;

		return *this;
	}


	KupljeniArtikal* GetArtikli(float ukupnaCijena, int& brojPronadjenih) {
		brojPronadjenih = 0;

		for (int i = 0; i < _trenutno; i++)
		{
			if (_artikli[i].GetCijena()*_artikli[i].GetKolicina() > ukupnaCijena) {
				brojPronadjenih++;
			}
		}

		if (brojPronadjenih == 0)
			return nullptr;

		KupljeniArtikal * rezArtikli = new KupljeniArtikal[brojPronadjenih];
		int indexNiz = 0;
		for (int i = 0; i < _trenutno; i++)
		{
			if (_artikli[i].GetCijena()*_artikli[i].GetKolicina() > ukupnaCijena) {
				rezArtikli[indexNiz++] = _artikli[i];
			}
		}

		return rezArtikli;
	}

	friend ostream& operator<<(ostream&, const Racun&);

};

ostream& operator<<(ostream& cout, const Racun& racun) {
	cout << "Broj racuna: " << racun._brojRacuna << endl;
	cout << "Naziv\tKol\tCijena\n\n";

	double iznos = 0;
	
	for (int i = 0; i < racun._trenutno; i++)
	{
		cout << racun._artikli[i] << endl;
		iznos += racun._artikli[i].GetKolicina()*racun._artikli[i].GetCijena();
	}

	cout << "Iznos racuna: " << iznos << endl;

	return cout;
}


int Racun::_brojacRacuna = 0;





int main() {
	KupljeniArtikal cokolada("Milka", 2.29);//naziv i cijena artikla
	KupljeniArtikal cigarete("Drina", 4.32);
	KupljeniArtikal mlijeko("Meggle", 2.87);
	KupljeniArtikal sok("Juicy", 1.38);

	cokolada += 0.27;//povecavamo cijenu artikla, sada je cijena 2,56BAM
	++cokolada;//povecavamo kolicinu artikla, sada je kolicina 2
	++sok;

	Racun racun;
	racun += cokolada;//dodajemo 2 cokolade po cijeni od 2,56BAM
	racun += sok;
	racun += sok;//ako vise puta pokusamo dodati isti artikal, treba da se uveca samo
				 //njegova kolicina na racunu tj.artikli se ne smiju duplirati
	racun += cigarete;
	//poziv u pozadini: racun.operator+=(mlijeko)
	racun += mlijeko;

	//poziv u pozadini: operator<<(cout, racun)
	cout << racun << endl; // ispis racuna mozete vidjeti na slici.

	int brojPronadjenih = 0;
	float ukupnaCijena = 5.10;
	cout << crt << "Artikli skuplji od >> " << ukupnaCijena << crt;
	//treba da vrati sve artikle cija je ukupna cijena na racunu (cijena * kolicina)
	//veca od poslane tj. 5.10BAM

	KupljeniArtikal * artikli = racun.GetArtikli(ukupnaCijena, brojPronadjenih);
	if (artikli != nullptr) {
		for (size_t i = 0; i < brojPronadjenih; i++) {
			cout << artikli[i] << endl;
		}
	}

	delete[] artikli;
	artikli = nullptr;

	system("pause");
	return 0;
}