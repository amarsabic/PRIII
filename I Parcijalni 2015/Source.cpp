/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR CE BITI OZNACENO KAO "RE"
3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA
4. PROGRAMSKI CODE SE TAKODJER NALAZI U FAJLU CODE_.TXT
****************************************************************************/
#include<iostream>
using namespace std;
const char *crt = "\n--------------------------------------------------\n";

char * AlocirajNizKaraktera(const char* sadrzaj) {
	if (sadrzaj == nullptr) return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

template<class T1, class T2>
class Kolekcija {
	T1 * _elementi1; // Student * elementi1; int * elementi2;
	T2 * _elementi2;
	int _trenutnoElemenata;
public:
	Kolekcija() {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutnoElemenata = 0;
	}

	~Kolekcija() {
		delete[] _elementi1; _elementi1 = nullptr;
		delete[] _elementi2; _elementi2 = nullptr;
	}
	Kolekcija(const Kolekcija& original) {
		_elementi1 = new T1[original._trenutnoElemenata];
		_elementi2 = new T2[original._trenutnoElemenata];

		for (int i = 0; i < original._trenutnoElemenata; i++)
		{
			_elementi1[i] = original._elementi1[i];
			_elementi2[i] = original._elementi2[i];
		}
		_trenutnoElemenata = original._trenutnoElemenata;
	}

	Kolekcija& operator=(const Kolekcija& original) {
		if (this != &original) {
			delete[] _elementi1; delete[] _elementi2;
			_elementi1 = new T1[original._trenutnoElemenata];
			_elementi2 = new T2[original._trenutnoElemenata];

			for (int i = 0; i < original._trenutnoElemenata; i++)
			{
				_elementi1[i] = original._elementi1[i];
				_elementi2[i] = original._elementi2[i];
			}
			_trenutnoElemenata = original._trenutnoElemenata;
		}
		return *this;
	}

	bool AddElement(T1 el1, T2 el2)
	{
		for (int i = 0; i < _trenutnoElemenata; i++)
		{
			if (_elementi1[i] == el1 && _elementi2[i] == el2) //operator poredjenja za T1 i T2
				return false;
		}

		T1* temp1 = new T1[_trenutnoElemenata + 1]; //defaultni konstruktor
		T2* temp2 = new T2[_trenutnoElemenata + 1];

		for (int i = 0; i < _trenutnoElemenata; i++)
		{
			temp1[i] = _elementi1[i]; //operator dodjele za T1 i T2
			temp2[i] = _elementi2[i];

		}
		delete[] _elementi1;
		delete[] _elementi2;

		temp1[_trenutnoElemenata] = el1;
		temp2[_trenutnoElemenata] = el2;
		_trenutnoElemenata++;

		_elementi1 = temp1;
		_elementi2 = temp2;

		return true;
	}
	void RemoveElement(int indeks) {
		if (indeks >= 0 || indeks < _trenutnoElemenata) {
			T1* temp1 = new T1[_trenutnoElemenata - 1]; //defaultni konstruktor
			T2* temp2 = new T2[_trenutnoElemenata - 1];

			for (int i = 0, j = 0; i < _trenutnoElemenata; i++)
			{
				if (i != indeks) {
					temp1[j] = _elementi1[i]; //operator dodjele za T1 i T2
					temp2[j] = _elementi2[i];
					j++;
				}
			}
			delete[] _elementi1;
			delete[] _elementi2;


			_elementi1 = temp1;
			_elementi2 = temp2;

			_trenutnoElemenata--;
		}
	}
	int getTrenutno() const {
		return _trenutnoElemenata;
	}

	T1& operator[](int indeks) {
		return _elementi1[indeks];
	}
	T2& GetElement2(int indeks) {
		return _elementi2[indeks];
	}

	void SortirajT2() {
		bool promjena = true;
		while (promjena) {
			promjena = false;
			for (int i = 0; i < _trenutnoElemenata - 1; i++)
			{
				if (_elementi2[i] > _elementi2[i + 1]) {
					T1 temp = _elementi1[i];
					_elementi1[i] = _elementi1[i + 1];
					_elementi1[i + 1] = temp;
					T2 temp1 = _elementi2[i];
					_elementi2[i] = _elementi2[i + 1];
					_elementi2[i + 1] = temp1;
					promjena = true;
				}
			}
		}
	}


	friend ostream& operator<<<>(ostream&, const Kolekcija&);
};

template<class T1, class T2>
ostream& operator<<<>(ostream& cout, const Kolekcija<T1, T2>& k) {
	for (int i = 0; i < k._trenutnoElemenata; i++)
	{
		cout << k._elementi1[i] << endl;//T1 treba imati preklopljen operator za ispit
		cout << k._elementi2[i] << endl;//T2 treba imati preklopljen operator za ispit
	}
	return cout;
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
			*_dan = *original._dan;
			*_mjesec = *original._mjesec;
			*_godina = *original._godina;
		}
		return *this;
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	friend ostream& operator<< (ostream &COUT, const Datum& obj) {
		COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina;
		return COUT;
	}

	friend bool operator==(const Datum&, const Datum&);
};
bool operator==(const Datum& d1, const Datum& d2) {
	return *d1._dan == *d2._dan && *d1._mjesec == *d2._mjesec && *d1._godina == *d2._godina;
}

class Student {
	char * _imePrezime;
	Datum * _datumRodjenja;
public:
	Student(const char * imePrezime = "---", Datum d = Datum()) {
		_imePrezime = AlocirajNizKaraktera(imePrezime);
		_datumRodjenja = new Datum(d);
	}
	Student(const Student& original) {
		_imePrezime = AlocirajNizKaraktera(original._imePrezime);
		_datumRodjenja = new Datum(*original._datumRodjenja);
	}

	~Student() { delete[] _imePrezime; delete _datumRodjenja; }

	Student& operator=(const Student& original) {
		if (this != &original) {
			delete[] _imePrezime;
			_imePrezime = AlocirajNizKaraktera(original._imePrezime);

			*_datumRodjenja = *original._datumRodjenja; //operator dodjele za datum
		}
		return *this;
	}

	friend ostream& operator<<(ostream&, const Student&);
	friend bool operator==(const Student&, const Student&);
};

ostream& operator<<(ostream& cout, const Student& student) {
	cout << "Ime i prezime: " << student._imePrezime << endl;
	return cout;
}
bool operator==(const Student& s1, const Student& s2) {
	return strcmp(s1._imePrezime, s2._imePrezime) == 0 && *s1._datumRodjenja == *s2._datumRodjenja;
}

class Ispit {
	Datum _datumOdrzavanja;
	char * _opisIspita;
	Kolekcija<Student, Datum> _prijave;
	Kolekcija<Student, int> _rezultati;
public:
	Ispit(const char* opis, Datum& datum) : _datumOdrzavanja(datum) {
		_opisIspita = AlocirajNizKaraktera(opis);
	}

	Ispit(const Ispit& ispit) :_datumOdrzavanja(ispit._datumOdrzavanja), _prijave(ispit._prijave), _rezultati(ispit._rezultati) {
		_opisIspita = AlocirajNizKaraktera(ispit._opisIspita);
	}

	~Ispit() {
		delete[] _opisIspita;
		_opisIspita = nullptr;
	}
	bool DodajPrijavu(Student& student, Datum& datum) {
		return _prijave.AddElement(student, datum);
	}
	void DodajRezultat(Student& student, int ocjena) {

		for (int i = 0; i < _rezultati.getTrenutno(); i++)
		{
			if (_rezultati[i] == student) {
				_rezultati.GetElement2(i) = ocjena;
				return;
			}
		}

		for (int i = 0; i < _prijave.getTrenutno(); i++)
		{
			if (_prijave[i] == student) {
				_rezultati.AddElement(student, ocjena);
				return;
			}
		}
	}

	void ZakljuciRezultate() {

		int indeks;

		for (int i = 0; i < _prijave.getTrenutno(); i++)
		{
			indeks = -1;
			for (int j = 0; j < _rezultati.getTrenutno(); j++)
			{
				if (_prijave[i] == _rezultati[j]) {
					indeks = i;
					break;
				}
			}

			if (indeks == -1) {
				_rezultati.AddElement(_prijave[i], 5);
			}
		}
	}

	void Sortiraj() {
		_rezultati.SortirajT2();
		cout << "Rezultati su sortirani\n";
	}

	friend ostream& operator<<(ostream&, const Ispit&);
};

ostream& operator<<(ostream& cout, const Ispit& ispit) {
	cout << "Datum odrzavanja: " << ispit._datumOdrzavanja << endl;
	cout << "Opis: " << ispit._opisIspita << endl;

	cout << "Prijave\n" << ispit._prijave << endl;
	cout << "Rezultati\n" << endl;

	if (ispit._prijave.getTrenutno() == ispit._rezultati.getTrenutno()) {
		cout << ispit._rezultati << endl;
	}

	return cout;
}



void main() {

	//REGIONI olaksavaju organizaciju programskog code-a, te ih mozete ukloniti ukoliko vam otezavaju preglednost

#pragma region TestiranjeKolekcije
	Kolekcija<int, double> kolekcija1;
	for (size_t i = 0; i < 50; i++)
		kolekcija1.AddElement(i, i + 0.3);
	cout << kolekcija1 << endl;
	cout << crt;
	Kolekcija<int, double> kolekcija2;
	kolekcija2 = kolekcija1;
	kolekcija1.RemoveElement(2);//UMANJITI VELIČINU NIZA
	cout << kolekcija2 << crt;
	if (kolekcija1.getTrenutno() == kolekcija2.getTrenutno())
		cout << "ISTI BROJ ELEMENATA" << endl;
	Kolekcija<int, double> kolekcija3(kolekcija2);
	cout << kolekcija3 << crt;
#pragma endregion

#pragma region TestiranjeDatuma
	Datum danas(26, 11, 2015);
	Datum sutra(danas);
	Datum prekosutra;
	prekosutra = danas;
	cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion

#pragma region TestiranjeStudenta
	Student denis("Denis Music", Datum(26, 11, 1990));
	Student jasmin("Jasmin Azemovic", Datum(22, 11, 1990));
	Student goran;
	goran = jasmin;
	Student adel("Adel Handzic", Datum(25, 8, 1990));
	Student marija("Marija Herceg", Datum(15, 6, 1990));
#pragma endregion

#pragma region TestiranjeIspita
	Ispit prIII("PRIII::26.11.2015", danas);
	//DodajPrijavu - dodaje novog studenta u listu prijavljenih za ispit. onemoguciti dodavanje dva ista studenta
	if (prIII.DodajPrijavu(denis, danas))
		cout << denis << " DODAT na spisak" << endl;
	if (prIII.DodajPrijavu(jasmin, danas))
		cout << jasmin << " DODAT na spisak" << endl;
	if (prIII.DodajPrijavu(goran, danas))
		cout << goran << " DODAT na spisak" << endl;
	if (prIII.DodajPrijavu(adel, danas))
		cout << adel << " DODAT na spisak" << endl;
	if (prIII.DodajPrijavu(marija, danas))
		cout << marija << " DODAT na spisak" << endl;

	cout << crt << endl;
	/*DodajRezultat - dodaje ocjenu koju je student ostvario na ispitu. za dodavanje ocjene student je prethodno trebao biti prijavljen na ispit
	*/
	prIII.DodajRezultat(goran, 7);
	prIII.DodajRezultat(jasmin, 6);
	prIII.DodajRezultat(adel, 8);
	prIII.DodajRezultat(marija, 9);

	///*Sortiraj - sortira rezultate ispita prema ocjeni*/
	prIII.Sortiraj();
	///*ZakljuciRezultate - dodaje ocjenu 5 svim studentima koji su prijavili ispit a nije im evidentirana ocjena*/
	prIII.ZakljuciRezultate();
	//rezultati ispita se smiju ispisivati jedino ukoliko su prethodno zaključeni
	cout << prIII << crt;

	Ispit prIII_2(prIII);
	cout << prIII_2 << crt;
#pragma endregion

	system("pause>null");
}

