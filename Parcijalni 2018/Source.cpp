#include <iostream>
/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR CE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URAĐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOCNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
6. ZA POTREBE TESTIRANJA, UNUTAR MAIN FUNKCIJE MOZETE DODAVATI NOVE TESTNE PODATKE
****************************************************************************/
using namespace std;

const char * crt = "\n------------------------------------\n";

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

class Datum {
	int *_dan, *_mjesec, *_godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum& original) {
		_dan = new int(*original._dan);
		_mjesec = new int(*original._mjesec);
		_godina = new int(*original._godina);
	}
	Datum& operator=(const Datum& original) {
		if (this != &original) {
			delete _dan; delete _mjesec; delete _godina;
			_dan = new int(*original._dan);
			_mjesec = new int(*original._mjesec);
			_godina = new int(*original._godina);
		}
		return *this;
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	friend ostream& operator<< (ostream &COUT, const Datum &obj) {
		COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina;
		return COUT;
	}

	int sumaDana() {
		int suma = 0;
		suma = *_godina * 365 + *_mjesec * 30 + *_dan;
		return suma;
	}
};

template<class T>
class Kolekcija {
	T * _niz;
	int _trenutno;
public:
	Kolekcija() : _trenutno(0), _niz(nullptr) {}

	Kolekcija(const Kolekcija& original) {
		_niz = new T[original._trenutno];
		for (int i = 0; i < original._trenutno; i++)
		{
			_niz[i] = original._niz[i];
		}
		_trenutno = original._trenutno;
	}

	Kolekcija& operator=(const Kolekcija& original) {
		if (this != &original) {
			delete[] _niz;
			_niz = new T[original._trenutno];
			for (int i = 0; i < original._trenutno; i++)
			{
				_niz[i] = original._niz[i];
			}
			_trenutno = original._trenutno;
			return *this;
		}
	}
	~Kolekcija() { delete[] _niz; _niz = nullptr; _trenutno = 0; }
	T * GetNiz() { return _niz; }
	int GetTrenutno() const { return _trenutno; }
	//implementirati potrebne funkcije

	T& getElement(int indeks) {
		return _niz[indeks];
	}
	T& operator[](int indeks) {
		return _niz[indeks];
	}

	bool Add(const T& el) {

		for (int i = 0; i < _trenutno; i++)
		{
			if (_niz[i] == el)
				return false;
		}

		T* temp = new T[_trenutno + 1];
		for (int i = 0; i < _trenutno; i++)
		{
			temp[i] = _niz[i];
		}
		delete[] _niz;
		temp[_trenutno] = el;
		_niz = temp;
		_trenutno++;
		return true;
	}

	friend ostream& operator<<<>(ostream&, const Kolekcija<T>&);
};
template<class T>
ostream& operator<<<>(ostream& cout, const Kolekcija<T>& kol) {
	for (int i = 0; i < kol.GetTrenutno(); i++)
	{
		cout << kol._niz[i] << endl;
	}
	return cout;
}

class Usluga {
	char* _naziv;
	double _cijena;//cijena usluge je izrazena po danu koristenja
public:
	Usluga() : _naziv(nullptr), _cijena(0) {}
	Usluga(const char* naziv, double cijena) : _cijena(cijena) {
		_naziv = AlocirajNizKaraktera(naziv);
	}
	Usluga& operator=(const Usluga& original) {
		if (this != &original) {
			_naziv = AlocirajNizKaraktera(original._naziv);
			_cijena = original._cijena;
		}
		return *this;
	}
	Usluga(const Usluga && original) {
		_naziv = AlocirajNizKaraktera(original._naziv);
		_cijena = original._cijena;
	}
	~Usluga() { delete[] _naziv; _naziv = nullptr; }
	double GetCijena() { return _cijena; }
	const char * GetNaziv() { return _naziv; }
	friend ostream& operator<< (ostream &COUT, const Usluga &obj);
	bool operator==(const Usluga& usluga) {
		return (strcmp(_naziv, usluga._naziv) == 0);
	}
};
ostream& operator<< (ostream &COUT, const Usluga &obj) {
	COUT << obj._naziv << " -> \t\t" << obj._cijena << " KM" << endl;
	return COUT;
}

class Rezervacija {
	static int _rezervacijeIDBrojac;
	const int _rezervacijaID; ///broj rezervacije krece od 160000
	Datum _rezervacijaOD;
	Datum _rezervacijaDO;
	double _cijenaPoDanu; //cijena osnovnih usluga koja ne ukljucuje dodatne usluge
	Kolekcija<Usluga> _dodatneUsluge; //dodatne usluge mogu biti: sauna, sportski tereni, kuglanje, masaza i sl.  // Usluga * _dodatneUsluge;
public:
	//implementirati potrebne funkcije

	Rezervacija(const Datum& datum1, const Datum& datum2, double cijenaPoDanu) :_rezervacijaOD(datum1), _rezervacijaDO(datum2), _rezervacijaID(_rezervacijeIDBrojac++) {
		_cijenaPoDanu = cijenaPoDanu;
	}

	bool operator+=(const Usluga& usluga) {
		for (int i = 0; i < _dodatneUsluge.GetTrenutno(); i++)
		{
			if (_dodatneUsluge[i] == usluga)
				return false;
		}
		 _dodatneUsluge.Add(usluga);
	}

	void PrintRacun() {
		///*prikazuje racun formatiran na nacin prikazan na slici. prilikom izracunavanja iznosa koristiti cijene osnovnih i dodatnih usluga, 
	///kao i broj dana za koje je napravljena rezervacija (napomena: duzina trajanja rezervacije je maksimalno 30 dana)*/

		/*
R.ID: 160000
OD: 25 10 2018
DO: 30 10 2018
DODATNE USLUGE:
Parking -> 5 KM
Dorucak -> 10 KM
Rucak -> 15 KM
Veseraj -> 10 KM
Osnovne usluge-> 299.5 KM
Dodatne usluge-> 200 KM
Ukupno -> 499.5 KM
*/

		cout << "ID rezervacije--> " << _rezervacijaID << endl;
		int sumaOD = _rezervacijaOD.sumaDana();
		int sumaDO = _rezervacijaDO.sumaDana();
		int brojDana = sumaDO - sumaOD;
		float Racun = 0;
		float dodatneUsluge = 0;
		cout << "OD " << _rezervacijaOD << endl;
		cout << "DO " << _rezervacijaDO << endl;
		cout << "Dodatne usluge:" << endl;
		for (int i = 0; i < _dodatneUsluge.GetTrenutno(); i++)
		{
			cout << _dodatneUsluge[i].GetNaziv() << "  " << _dodatneUsluge[i].GetCijena() << endl;
			Racun += _dodatneUsluge[i].GetCijena()*brojDana; //dodavanje usluge * brojDana
			dodatneUsluge = Racun;
		}
		cout << "Dodatne usluge--> " << dodatneUsluge << endl;
		cout << "Osnovne usluge--> " << brojDana * _cijenaPoDanu << endl;
		Racun += brojDana * _cijenaPoDanu;
		cout << "Cijena po nocenju: " << _cijenaPoDanu << endl;
		cout << "Ukupan iznos za " << brojDana << " dana iznosi: " << Racun << endl;
	}

	Usluga * operator[](const char* naziv) {
		//vraca pokazivac na uslugu sa proslijedjenim nazivom

		Usluga temp;

		for (int i = 0; i < _dodatneUsluge.GetTrenutno(); i++)
		{
			if (strcmp(naziv, _dodatneUsluge[i].GetNaziv()) == 0) {
				return &_dodatneUsluge[i];
			}
		}
		return nullptr;
	}

	Kolekcija<Usluga> operator()(int OD, int DO) {
		//vraca sve dodatne usluge koje imaju cijenu u opsegu vrijednosti parametara

		Kolekcija<Usluga> temp;

		for (int i = 0; i < _dodatneUsluge.GetTrenutno(); i++)
		{
			if (_dodatneUsluge[i].GetCijena() >= OD && _dodatneUsluge[i].GetCijena() <= DO) {
				temp.Add(_dodatneUsluge[i]);
			}
		}
		return temp;
	}
};

int Rezervacija::_rezervacijeIDBrojac = 160000;

void main() {
	cout << crt << "TRAJANJE ISPITA -> 90 minuta " << crt;
	Kolekcija<int> brojevi;
	for (int i = 0; i < 15; i++)
		brojevi.Add(i);
	cout << brojevi << crt;

	//u okviru klase Usluga implementirati MOVE konstruktor
	Usluga Parking("Parking", 5), Dorucak("Dorucak", 10), Rucak("Rucak", 15), Veseraj("Veseraj", 10);

	//rezervacijaOD, rezervacijaDO, cijenaPoDanu
	Rezervacija rezervacija(Datum(25, 10, 2018), Datum(30, 10, 2018), 59.9);

	//dodaje novu uslugu u rezervaciju. onemoguciti ponavljanje usluga
	if (rezervacija += Parking)
		cout << "Usluga uspjesno dodana!" << endl;
	if (rezervacija += Dorucak)
		cout << "Usluga uspjesno dodana!" << endl;
	if (rezervacija += Rucak)
		cout << "Usluga uspjesno dodana!" << endl;
	if (rezervacija += Veseraj)
		cout << "Usluga uspjesno dodana!" << endl;
	if (!(rezervacija += Dorucak))
		cout << "Usluga vec postoji!" << endl;

	///*prikazuje racun formatiran na nacin prikazan na slici. prilikom izracunavanja iznosa koristiti cijene osnovnih i dodatnih usluga, 
	///kao i broj dana za koje je napravljena rezervacija (napomena: duzina trajanja rezervacije je maksimalno 30 dana)*/
	rezervacija.PrintRacun();

	//vraca pokazivac na uslugu sa proslijedjenim nazivom
	Usluga * p = rezervacija["Dorucak"];
	if (p != nullptr)
		cout << *p << crt;

	//vraca sve dodatne usluge koje imaju cijenu u opsegu vrijednosti parametara
	Kolekcija<Usluga> uslugePoCijeni = rezervacija(8, 12);
	for (size_t i = 0; i < uslugePoCijeni.GetTrenutno(); i++)
		cout << uslugePoCijeni[i] << crt;

	cin.get();
	system("pause>null");
}

/*
R.ID: 160000
OD: 25 10 2018
DO: 30 10 2018
DODATNE USLUGE:
Parking -> 5 KM
Dorucak -> 10 KM
Rucak -> 15 KM
Veseraj -> 10 KM
Osnovne usluge-> 299.5 KM
Dodatne usluge-> 200 KM
Ukupno -> 499.5 KM
*/