#include<iostream>
#include<string>
#include<exception>
#include<sstream>
#include<regex>
#include<vector>
#include<iterator>

using namespace std;

const char* crt = "\n-----------------------------------------------------------\n";

char * AlocirajNizKaraktera(const char* sadrzaj) {
	if (sadrzaj == nullptr) return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}


enum OznakaKrvneGrupe { O, A, B, AB };
const char* OznakaKrvneGrupeString[] = { "O", "A", "B", "AB" };
ostream& operator<<(ostream& cout, const OznakaKrvneGrupe& oznaka) {
	if (oznaka == 0) cout << "O";
	if (oznaka == 1) cout << "A";
	if (oznaka == 2) cout << "B";
	if (oznaka == 3) cout << "AB";
	return cout;
}

class Datum {
	int _dan, _mjesec, _godina;
public:
	Datum(int dan, int mjesec, int godina) :
		_dan(dan), _mjesec(mjesec), _godina(godina) {}

	string ToString(string separator = ".") {
		stringstream ss;
		ss << _dan << separator << _mjesec << separator << _godina << endl;
		return ss.str();
	}
};

template <class T1, class T2, int max>
class Kolekcija {
	T1 _elementi1[max];
	T2 _elementi2[max];
	int _trenutnoElemenata;
	int _keyElement; // kljucni element, a podrazumijevano T1 predstavlja kljucni
	//element i tada je vrijednost 1, a u slucaju da je T2 onda ce vrijednost biti 2
public:
	Kolekcija(int keyElement = 1) {
		_trenutnoElemenata = 0;
		_keyElement = keyElement;
	}
	bool AddElement(T1 ele1, T2 ele2) {
		if (_trenutnoElemenata == max)
			return false;
		_elementi1[_trenutnoElemenata] = ele1;
		_elementi2[_trenutnoElemenata] = ele2;
		_trenutnoElemenata++;
		return true;
	}
	void Sort(string sort) {
		bool promjena = true;
		while (promjena) {
			promjena = false;
			for (int i = 0; i < _trenutnoElemenata - 1; i++)
			{
				if (sort == "ASC" && _keyElement == 1 && _elementi1[i] > _elementi1[i + 1]) {
					swap<T1, T2>(_elementi1[i], _elementi1[i + 1]);
					swap<T1, T2>(_elementi2[i], _elementi2[i + 1]);
					promjena = true;
				}
				else if (sort == "DESC" && _keyElement == 1 && _elementi1[i] < _elementi1[i + 1]) {
					swap<T1, T2>(_elementi1[i], _elementi1[i + 1]);
					swap<T1, T2>(_elementi2[i], _elementi2[i + 1]);
					promjena = true;
				}
				else if (sort == "ASC" && _keyElement == 2 && _elementi2[i] > _elementi2[i + 1]) {
					swap<T1, T2>(_elementi1[i], _elementi1[i + 1]);
					swap<T1, T2>(_elementi2[i], _elementi2[i + 1]);
					promjena = true;
				}
				else if (sort == "DESC" && _keyElement == 2 && _elementi2[i] < _elementi2[i + 1]) {
					swap<T1, T2>(_elementi1[i], _elementi1[i + 1]);
					swap<T1, T2>(_elementi2[i], _elementi2[i + 1]);
					promjena = true;
				}
			}
		}
	}
	void SetElement2(int lokacija, T2 ele2) {
		_elementi2[lokacija] = ele2;
	}
	void PovecajKolicinu(int lokacija, int kolicina) {
		_elementi2[lokacija] += kolicina;
	}
	void SmanjiKolicinu(int lokacija, int kolicina) {
		_elementi2[lokacija] -= kolicina;
	}
	void SetKeyElement(int keyElement) {
		if (keyElement < 1 || keyElement>2)
			throw exception("Nedozvoljena vrijednost!");
		_keyElement = keyElement;
	}
	int GetTrenutno() const { return _trenutnoElemenata; }
	int GetMax() const { return max; }

	T1 GetElement1(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception("Nepostojeca lokacija!");
		return _elementi1[lokacija];
	}
	T2 GetElement2(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception("Nepostojeca lokacija!");
		return _elementi2[lokacija];
	}
	friend ostream &operator<<(ostream &COUT, const Kolekcija &obj) {
		for (size_t i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj.GetElement1(i) << " " << obj.GetElement2(i) << endl;
		return COUT;
	}
};

class KrvnaGrupa {
	/*postoje 4 krvne grupe koje su navedene u enumeracije OznakaKrvneGrupe,
	pri cemu svaka od navedenih moze imati pozitivan (+) i negativan (-) Rh faktor*/
	OznakaKrvneGrupe _oznaka;//npr: AB
	char _rhFaktor; // + ili -
					/*prethodno pomenuto je izuzetno bitno iz razloga sto postoje jasna pravila vezana za darivanje
					krvi tj. koji primalac moze primiti krv od kojeg donatora sto je prikazano u tabeli, a naredna dva vector-a
					su zaduzena da cuvaju informacije o tome, npr. za krvnu grupu A+ vector donatori ce cuvati vrijednosti: A+ i AB+ */
	vector<KrvnaGrupa> _donatori; //krvne grupe kojima odredjena krvna grupa moze donirati krv.
	vector<KrvnaGrupa> _primaoci; //krvne grupe od kojih odredjena krvna grupa moze primiti krv.
public:
	KrvnaGrupa(OznakaKrvneGrupe oznaka, char rhFaktor) :
		_oznaka(oznaka), _rhFaktor(rhFaktor) {}

	string ToString() {
		stringstream ss;
		ss << "Krvna grupa -> " << _oznaka << _rhFaktor;
		ss << crt;
		ss << "Donori -> ";
		for (int i = 0; i < _donatori.size(); i++)
		{
			ss << _donatori[i]._oznaka << _donatori[i]._rhFaktor << ", ";
		}
		ss << endl;
		ss << "Donori -> ";
		for (int i = 0; i < _primaoci.size(); i++)
		{
			ss << _primaoci[i]._oznaka << _primaoci[i]._rhFaktor << ", ";
		}
		ss << crt;
		return ss.str();
	}

	bool operator==(const KrvnaGrupa& k) {
		return _oznaka == k._oznaka && _rhFaktor == k._rhFaktor;
	}
	vector<KrvnaGrupa>& GetPrimaoci() {
		return _primaoci;
	}
	vector<KrvnaGrupa>& GetDonatori() {
		return _donatori;
	}
	void SetDonatori(vector<KrvnaGrupa> donatori) {
		_donatori = donatori;
	}
	void SetPrimaoci(vector<KrvnaGrupa> primaoci) {
		_primaoci = primaoci;
	}
};

//Klasu proglasiti apstraktnom
class Osoba {
protected:
	char* _imePrezime;
	KrvnaGrupa _krvnaGrupa;
public:
	Osoba(const char* imePrezime, KrvnaGrupa krvnaGrupa) : _krvnaGrupa(krvnaGrupa) {
		_imePrezime = AlocirajNizKaraktera(imePrezime);
	}
	bool operator==(const Osoba& o) {
		return strcmp(_imePrezime, o._imePrezime) == 0;
	}
	Osoba(const Osoba& o) :_krvnaGrupa(o._krvnaGrupa) {
		_imePrezime = AlocirajNizKaraktera(o._imePrezime);
	}
	Osoba& operator=(const Osoba& o) {
		if (this != &o) {
			delete[] _imePrezime;
			_imePrezime = AlocirajNizKaraktera(_imePrezime);
			_krvnaGrupa = o._krvnaGrupa;
		}
		return *this;
	}
	KrvnaGrupa GetKrvnaGrupa() {
		return _krvnaGrupa;
	}
	~Osoba() {
		delete[] _imePrezime;
		_imePrezime = nullptr;
	}

	virtual void Info() = 0;
};

class Donator : public Osoba {
	//u slucaju da broj telefona nije u validnom formatu potrebno ga je postaviti na podrazumijevanu vrijednost: 000-000-000
	string _telefon; //regex: 000/000-000 ili 000-000-000
	Datum _datumPosljednjegDoniranja;
	bool _podsjetiMe;
	bool _kontaktirajMe;

	bool ValidanFormat(string telefon) {
		string pravilo = "([0-9]{3})(\\-\\/?)([0-9]{3})(\\-?)([0-9]{3})";
		if (regex_match(telefon, regex(pravilo)))
			return true;
		return false;
	}

public:
	Donator(const char* imePrezime, KrvnaGrupa krvnaGrupa, string telefon, Datum dpd, bool remind = true, bool contact = true)
		: Osoba(imePrezime, krvnaGrupa), _datumPosljednjegDoniranja(dpd) {
		if (ValidanFormat(telefon))
			_telefon = telefon;
		else
			_telefon = "000-000-000";
	}
	void SetDatumDoniranja(Datum datum) {
		_datumPosljednjegDoniranja = datum;
	}
	void Info() {
		cout << *this;
	}
	friend ostream& operator<<(ostream& cout, Donator& d) {
		cout << "Ime i prezime donatora -> " << d._imePrezime << endl;
		cout << "Datum zadnjeg doniranja -> " << d._datumPosljednjegDoniranja.ToString() << endl;
		return cout;
	}
};

class Zahtjev {
	string _ustanova;
	Datum _datumZahtjeva;
	KrvnaGrupa _krvnaGrupa;
	double _kolicina;
public:
	Zahtjev(string ustanova, Datum datum, KrvnaGrupa krvnaGrupa, double kolicina) :
		_ustanova(ustanova), _datumZahtjeva(datum), _krvnaGrupa(krvnaGrupa), _kolicina(kolicina) { }

	KrvnaGrupa GetKrvnaGrupa() {
		return _krvnaGrupa;
	}
	double GetKolicina() {
		return _kolicina;
	}

	friend ostream& operator<<(ostream& COUT, Zahtjev& obj) {
		COUT << "Ustanova: " << obj._ustanova << endl;
		COUT << "Datum: " << obj._datumZahtjeva.ToString() << endl;
		//COUT << "Krvna grupa: " << obj._krvnaGrupa.ToString() << endl;
		COUT << "Kolicina: " << obj._kolicina << endl;
		return COUT;
	}
};

class TransfuzijskiCentar {
	//stanje zaliha za svaku pojedinu krvnu grupu
	Kolekcija<KrvnaGrupa*, double, 8> _zalihe;
	//evidentira svaku donaciju krvi
	Kolekcija<Osoba *, double, 100> _donacije;
	vector<Zahtjev> _zahtjevi;
public:

	bool AddDonaciju(Datum datumDonacije, Osoba* osoba, int kolicina) {
		bool imaZaliha = false;
		for (int i = 0; i < _zalihe.GetTrenutno(); i++)
		{
			if (*_zalihe.GetElement1(i) == osoba->GetKrvnaGrupa()) {
				_zalihe.PovecajKolicinu(i, kolicina);
				imaZaliha = true;
			}
		}
		if (!imaZaliha)
			_zalihe.AddElement(new KrvnaGrupa(osoba->GetKrvnaGrupa()), kolicina);

		for (int i = 0; i < _donacije.GetTrenutno(); i++)
		{
			if (*_donacije.GetElement1(i) == *osoba) {
				dynamic_cast<Donator*>(_donacije.GetElement1(i))->SetDatumDoniranja(datumDonacije);
				_donacije.PovecajKolicinu(i, kolicina);
				return true;
			}
		}

		_donacije.AddElement(new Donator(*dynamic_cast<Donator*>(osoba)), kolicina);
		return true;
	}

	bool DodajZahtjev(Zahtjev zahtjev) {
		bool imaZaliha = false;
		for (int i = 0; i < _zalihe.GetTrenutno(); i++)
		{
			for (int j = 0; j < zahtjev.GetKrvnaGrupa().GetPrimaoci().size(); j++)
			{
				if (*_zalihe.GetElement1(i) == zahtjev.GetKrvnaGrupa().GetPrimaoci()[j] && _zalihe.GetElement2(i) >= zahtjev.GetKolicina()) {
					imaZaliha = true;
					_zalihe.SmanjiKolicinu(i, zahtjev.GetKolicina());
					break;
				}
			}
		}
		_zahtjevi.push_back(zahtjev);
		return true;
	}
};

void main() {

	Datum dat_12_01_2018(12, 1, 2018), dat_01_02_2018(1, 2, 2018);
	cout << dat_01_02_2018.ToString() << endl; //podrazumijevani separator je tacka ".", pa ispisuje 1.2.2018
	cout << dat_12_01_2018.ToString("/") << endl; //separator je proslijedjen, pa ispisuje 12/1/2018

	Kolekcija<int, int, 10> kolekcija1;
	for (size_t i = 0; i < kolekcija1.GetMax(); i++)
		if (!kolekcija1.AddElement(i, 170000 + i))
			cout << "Elementi " << i << " i " << 170000 + i << " nisu dodati u kolekciju" << endl;

	cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) << endl;

	kolekcija1.Sort("ASC"); //kljucne rijeci za sortiranje su ASC i DESC, a sortiranje se vrsi prema kljucnom elementu
	cout << kolekcija1 << endl << endl << endl;
	kolekcija1.Sort("DESC");
	cout << kolekcija1 << endl;

	kolekcija1.SetKeyElement(2);//postavlja kljucni element na T2

	kolekcija1.Sort("DESC"); //sada se sortiranje vrsi po elementu T2
	cout << kolekcija1 << endl;

	KrvnaGrupa
		O_poz(O, '+'), O_neg(O, '-'),
		A_poz(A, '+'), A_neg(A, '-'),
		B_poz(B, '+'), B_neg(B, '-'),
		AB_poz(AB, '+'), AB_neg(AB, '-');

	vector<KrvnaGrupa> donori_O_poz{ O_poz, A_poz, B_poz, AB_poz };
	O_poz.SetDonatori(donori_O_poz);
	//ili krace napisano
	O_poz.SetPrimaoci(vector<KrvnaGrupa>{O_poz, O_neg});
	A_poz.SetDonatori(vector<KrvnaGrupa>{A_poz, AB_poz});
	A_poz.SetPrimaoci(vector<KrvnaGrupa>{O_poz, A_poz, O_neg, A_neg});

	B_poz.SetDonatori(vector<KrvnaGrupa>{B_poz, AB_poz});
	B_poz.SetPrimaoci(vector<KrvnaGrupa>{O_poz, B_poz, O_neg, B_neg});
	/*
	Ispis podataka o krvnoj grupi treba biti u formatu:
	-------------------------------------------
	Krvna grupa -> 0+
	-------------------------------------------
	Donori  ->  0+, A+, B+, AB+
	Primaoci ->	0+, 0-
	-------------------------------------------
	*/
	cout << O_poz.ToString() << endl;

	Osoba * jasmin = new Donator("Jasmin Azemovic", B_poz, "061-111-222", Datum(12, 2, 2017), true, true);
	Osoba * adel = new Donator("Adel Handzic", A_neg, "061-222-333", Datum(9, 1, 2017), true, true);
	Osoba * goran = new Donator("Goran Skondric", B_neg, "061-333-444", Datum(9, 3, 2017), true, true);


	TransfuzijskiCentar tcMostar;
	/*
	prilikom svake donacije je potrebno povecati zalihe za tu krvnu grupu
	ukoliko krvna grupa vec postoji na zalihi onda se uvecava samo njena kolicina
	takodjer, donatoru se postavlja nova vrijednost datuma posljednje donacije
	*/
	//datum donacije, donator, kolicina
	tcMostar.AddDonaciju(Datum(20, 5, 2017), jasmin, 2.5);
	tcMostar.AddDonaciju(Datum(20, 5, 2017), adel, 3);
	tcMostar.AddDonaciju(Datum(20, 5, 2017), adel, 3);
	tcMostar.AddDonaciju(Datum(6, 5, 2017), goran, 1.2);


	tcMostar.AddDonaciju(Datum(10, 9, 2017), jasmin, 2);
	tcMostar.AddDonaciju(Datum(18, 10, 2017), adel, 1.8);
	tcMostar.AddDonaciju(Datum(15, 9, 2017), goran, 3.8);

	Zahtjev zahtjev_0_poz("Tranfuziologija KCUS", Datum(18, 2, 2018), O_poz, 15),
		zahtjev_0_neg("Tranfuziologija Bakir Nakas", Datum(20, 2, 2018), O_neg, 8);

	/*ukoliko transfuzijski centar posjeduje zahtijevane kolicine na stanju, odmah ce ih ustupiti prema zahtjevu,
	a u slucaju da ne posjeduje onda ce (koristeci multithreading) kontaktirati (poslati SMS poruku sa odgovarajucim sadrzajem)
	sve donore koji zadovoljavaju sljedece uslove:
	- mogu donirati krv zahtijevanoj krvnoj grupi
	- oznacili su da zele biti kontaktirani
	- nisu davali krv u posljednja 3 mjeseca
	*/
	tcMostar.DodajZahtjev(zahtjev_0_poz);
	tcMostar.DodajZahtjev(zahtjev_0_neg);

	//printa zahvalnice (zahvaljujemo se ime i prezime donatoru na ukupno doniranih X doza krvi) za TOP 2 donatora krvi
	//tcMostar.GetZahvalniceZahvalnice(2);

	delete adel;
	delete jasmin;
	delete goran;


	system("pause>null");
}