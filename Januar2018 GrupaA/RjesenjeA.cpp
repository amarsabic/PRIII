#include<iostream>
#include<vector>
#include<string>
#include<exception>
#include<sstream>
#include<iterator>
#include<regex>
#include<thread>
#include<mutex>

using namespace std;

mutex cuvar;

const char *crt = "\n-------------------------------------------\n";
enum GodinaStudija { PRVA = 1, DRUGA, TRECA };

char * Alociraj(const char * sadrzaj) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

template<class T1, class T2>
class Dictionary {
	T1 * _elementi1;
	T2 * _elementi2;
	int * _trenutno;
public:
	Dictionary() {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = new int(0);
	}
	Dictionary(const Dictionary& obj)
	{
		delete[]_elementi1; _elementi1 = nullptr;
		delete[]_elementi2; _elementi2 = nullptr;
		_elementi1 = new T1[*obj._trenutno];
		_elementi2 = new T2[*obj._trenutno];
		for (int i = 0; i < *obj._trenutno; i++)
		{
			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
		_trenutno = new int(*obj._trenutno);
	}
	Dictionary& operator=(const Dictionary& obj) {
		if (this != &obj) {
			delete[]_elementi1; _elementi1 = nullptr;
			delete[]_elementi2; _elementi2 = nullptr;
			_elementi1 = new T1[*obj._trenutno];
			_elementi2 = new T2[*obj._trenutno];
			for (int i = 0; i < *obj._trenutno; i++)
			{
				_elementi1[i] = obj._elementi1[i];
				_elementi2[i] = obj._elementi2[i];
			}
			*_trenutno = *obj._trenutno;
		}
		return *this;
	}
	bool AddElement(T1 el1, T2 el2) {
		T1* temp1 = new T1[*_trenutno + 1];
		T2* temp2 = new T2[*_trenutno + 2];
		for (int i = 0; i < *_trenutno; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		delete[] _elementi1;
		delete[] _elementi2;

		temp1[*_trenutno] = el1;
		temp2[*_trenutno] = el2;

		_elementi1 = temp1;
		_elementi2 = temp2;

		(*_trenutno)++;
		return true;
	}

	Dictionary<int, int> getRange(int pocetak, int kraj) {
		//vraca elemente kolekcije koji se nalaze na lokacijama definisanim vrijednostima parametara (npr. 2 - 7). 
		//funkcija baca izuzetak u slucaju da se zahtijeva lokacija koja ne postoji ili je vrijednost posljednje lokacije 
		//manja od pocetne

		int brojac = 0;
		Dictionary<int, int> novi;
		bool postojiP = false;
		bool postojiK = false;

		if (kraj < pocetak) throw exception("Pocetak manji od kraja");
		for (int i = 0; i < *_trenutno; i++)
		{
			if (pocetak == i)
				postojiP = true;
			if (kraj == i)
				postojiK = true;
		}
		if (!postojiP || !postojiK) throw exception("Lokacija ne postoji");

		for (int i = pocetak; i < kraj; i++)
		{
			novi.AddElement(_elementi1[i], _elementi2[i]);
		}

		return novi;
	}

	~Dictionary() {
		delete[]_elementi1; _elementi1 = nullptr;
		delete[]_elementi2; _elementi2 = nullptr;
		delete _trenutno; _trenutno = nullptr;
	}
	T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
	int getTrenutno() const { return *_trenutno; }
	friend ostream& operator<< (ostream &COUT, const Dictionary &obj) {
		for (size_t i = 0; i < *obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
};

class DatumVrijeme {
	int *_dan, *_mjesec, *_godina, *_sati, *_minuti;
public:
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);
	}
	int PretvoriMinute() {
		int suma = 0;
		suma = *_godina * 365 + *_minuti * 60 + *_dan + *_sati * 24 + *_minuti * 1440;
		return suma;
	}
	DatumVrijeme(const DatumVrijeme& obj)
	{
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
		_sati = new int(*obj._sati);
		_minuti = new int(*obj._minuti);
	}
	DatumVrijeme& operator=(const DatumVrijeme& obj)
	{
		if (this != &obj) {
			*_dan = *obj._dan;
			*_mjesec = *obj._mjesec;
			*_godina = *obj._godina;
			*_sati = *obj._sati;
			*_minuti = *obj._minuti;
		}
		return *this;
	}
	string ToCharArray() {
		stringstream ss;
		ss << *_dan << "/" << *_mjesec << "/" << *_godina << " " << *_sati << ":" << *_minuti << endl;
		return Alociraj(ss.str().c_str());
	}
	~DatumVrijeme() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;
	}
	friend ostream& operator<< (ostream &COUT, const DatumVrijeme &obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti << endl;
		return COUT;
	}

	int GetDateInHours() {
		int suma = 0;
		suma = *_godina * 8670 + *_mjesec * 744 + *_dan * 24 + *_sati;
		return suma;
	}
};

class Predmet {
	char * _naziv;
	int _ocjena;
	string _napomena;
public:
	Predmet(const char * naziv = "", int ocjena = 0, string napomena = "") {
		_naziv = Alociraj(naziv);
		_ocjena = ocjena;
		_napomena = napomena;
	}
	Predmet(const Predmet& p)
	{
		_naziv = Alociraj(p._naziv);
		_napomena = p._napomena;
		_ocjena = p._ocjena;
	}
	Predmet& operator=(const Predmet& p)
	{
		if (this != &p) {
			delete[] _naziv;
			_naziv = Alociraj(p._naziv);
			_napomena = p._napomena;
			_ocjena = p._ocjena;
		}
		return *this;
	}
	~Predmet() {
		delete[] _naziv; _naziv = nullptr;
	}
	friend ostream& operator<< (ostream &COUT, Predmet &obj) {
		COUT << obj._naziv << " (" << obj._ocjena << ") " << obj._napomena << endl;
		return COUT;
	}
	string GetNapomena() { return _napomena; }
	char * GetNaziv() { return _naziv; }
	int GetOcjena() { return _ocjena; }

	void DodajNapomenu(string napomena) {
		_napomena += " " + napomena;
	}
};

class Uspjeh {
	GodinaStudija * _godina;
	//datumvrijeme se odnosi na vrijeme evidentiranja polozenog predmeta
	Dictionary<Predmet, DatumVrijeme> _predmeti; //
public:
	Uspjeh(GodinaStudija godina) {
		_godina = new GodinaStudija(godina);
	}
	Uspjeh(const Uspjeh& u)
	{
		_godina = new GodinaStudija(*u._godina);
		if (u._predmeti.getTrenutno() > 0) {
			for (int i = 0; i < u._predmeti.getTrenutno(); i++)
			{
				_predmeti.AddElement(u._predmeti.getElement1(i), u._predmeti.getElement2(i));
			}
		}
	}
	Uspjeh& operator=(const Uspjeh& u) {
		if (this != &u) {
			*_godina = *u._godina;
			_predmeti = u._predmeti;
		}
		return *this;
	}
	void DodajPredmet(Predmet predmet, DatumVrijeme datum) {
		_predmeti.AddElement(predmet, datum);
	}
	~Uspjeh() { delete _godina; _godina = nullptr; }

	Dictionary<Predmet, DatumVrijeme> * GetPredmeti() { return &_predmeti; }
	GodinaStudija * GetGodinaStudija() { return _godina; }
	friend ostream& operator<< (ostream &COUT, const Uspjeh &obj) {
		COUT << "Godina: " << *obj._godina << " " << obj._predmeti << endl;
		return COUT;
	}
};

class Student {
	char * _imePrezime;
	string _emailAdresa;
	string _brojTelefona;
	vector<Uspjeh> _uspjeh;

	void PosaljiEmail(string poruka) {
		cout << poruka;
	}
public:
	Student(const char * imePrezime, string emailAdresa, string brojTelefona) {
		_imePrezime = Alociraj(imePrezime);
		string pravilo = "[a-zA-Z]*.[a-zA-Z]*@[hotmail.com|outlook.com|fit.ba]*";
		if (regex_match(emailAdresa, regex(pravilo))) {
			_emailAdresa = emailAdresa;
		}
		else {
			_emailAdresa = "notSet@fit.ba";
		}
		_brojTelefona = brojTelefona;
	}
	Student(const Student& s)
	{
		delete[] _imePrezime;
		_imePrezime = Alociraj(s._imePrezime);
		_emailAdresa = s._emailAdresa;
		_brojTelefona = s._brojTelefona;
	}
	Student& operator=(const Student& s)
	{
		if (this != &s) {
			delete[] _imePrezime;
			_imePrezime = Alociraj(s._imePrezime);
			_emailAdresa = s._emailAdresa;
			_brojTelefona = s._brojTelefona;
		}
		return *this;
	}

	vector<Predmet> operator()(DatumVrijeme* datumOD, DatumVrijeme* datumDO) {
		//vraca niz predmeta koji su evidentiranih u periodu izmedju vrijednosti proslijedjenih parametara
		vector<Predmet> p;
		for (int i = 0; i < _uspjeh.size(); i++) {
			for (int j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++)
			{
				if (_uspjeh[i].GetPredmeti()->getElement2(j).GetDateInHours() > datumOD->GetDateInHours() && _uspjeh[i].GetPredmeti()->getElement2(j).GetDateInHours() < datumDO->GetDateInHours()) {
					p.push_back(_uspjeh[i].GetPredmeti()->getElement1(j));
				}
			}
		}
		return p;
	}

	//Uspjeh * uspjeh_I_godina = jasmin["PRVA"];//vraca uspjeh Studenta ostvaren u prvoj godini studija
	Uspjeh* operator[](string stringGodina) {
		for (int i = 0; i < _uspjeh.size(); i++)
		{
			if (stringGodina == "PRVA" && *_uspjeh[i].GetGodinaStudija() == PRVA) return &_uspjeh[i];
			if (stringGodina == "DRUGA" && *_uspjeh[i].GetGodinaStudija() == DRUGA) return &_uspjeh[i];
			if (stringGodina == "TRECA" && *_uspjeh[i].GetGodinaStudija() == TRECA) return &_uspjeh[i];
		}
		return nullptr;
	}

	bool AddPredmet(Predmet predmet, GodinaStudija godinaStudija, DatumVrijeme datum) {
		for (int i = 0; i < _uspjeh.size(); i++)
		{
			if (*_uspjeh[i].GetGodinaStudija() == godinaStudija) {
				for (int j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++)
				{
					if (strcmp(_uspjeh[i].GetPredmeti()->getElement1(j).GetNaziv(), predmet.GetNaziv()) == 0)
						return false;
					if (datum.GetDateInHours() - _uspjeh[i].GetPredmeti()->getElement2(_uspjeh[i].GetPredmeti()->getTrenutno() - 1).GetDateInHours() < 1)
						return false;
				}
			}
		}
		Uspjeh temp(godinaStudija);
		temp.DodajPredmet(predmet, datum);
		_uspjeh.push_back(temp);

		thread t1([&]() {
			cuvar.lock();
			cout << "FROM:info@fit.ba" << endl << "TO: emailStudenta" << endl << "Postovani ime i prezime,  evidentirali ste uspjeh za X godinu studija." << endl;
			cout << "Pozdrav." << endl;
			cout << "FIT Team." << endl;
			cuvar.unlock();
		});
		thread t2([&]() {
			//ukoliko je, nakon dodavanja predmeta, prosjek na nivou te godine veci od 8.0 Studentu se, pored email - a,
				//salje i SMS sa sadrzajem: "Svaka cast za uspjeh X.X ostvaren u X godini studija".
				//slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
			float prosjek = 0;
			vector<Uspjeh>::iterator it;
			for (it = _uspjeh.begin(); it < _uspjeh.end(); it++)
			{
				if (*it->GetGodinaStudija() == godinaStudija) {
					for (int j = 0; j < it->GetPredmeti()->getTrenutno(); j++)
					{
						prosjek += it->GetPredmeti()->getElement1(j).GetOcjena();
					}
					prosjek /= it->GetPredmeti()->getTrenutno();
				}
			}
			if (prosjek > 8) {
				cuvar.lock();
				cout << "Saljem sms " << "Svaka cast za uspjeh " << prosjek << " ostvaren u X godini studija" << endl;
				cuvar.unlock();
			}
		});
		t1.join();
		t2.join();
		return true;
	}
	~Student() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	friend ostream& operator<< (ostream &COUT, Student &obj) {
		COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojTelefona << endl << endl;
		ostream_iterator<Uspjeh> out_iter(cout, "\n");
		copy(obj._uspjeh.begin(), obj._uspjeh.end(), out_iter);
		return COUT;
	}
	vector<Uspjeh> * GetUspjeh() { return &_uspjeh; }
	string GetEmail() { return _emailAdresa; }
	string GetBrojTelefona() { return _brojTelefona; }
	char * GetImePrezime() { return _imePrezime; }
};

void main() {

	cout << "NA KRAJU ISPITA, RAD PREDAJTE U ODGOVARAJUCI FOLDER NA FTP SERVERU (INTEGRALNI)!" << endl;
	DatumVrijeme temp,
		datum19062019_1015(19, 6, 2019, 10, 15),
		datum20062019_1115(20, 6, 2019, 11, 15),
		datum30062019_1215(30, 6, 2019, 12, 15),
		datum05072019_1231(5, 7, 2019, 12, 31);
	//funkcija ToCharArray vraca datum i vrijeme kao char *. konverziju izvrsiti koristeci stringstream objekat.
	//voditi racuna o tome da se izmedju datuma i vremena nalazi samo jedan razmak, te da su vrijednosti dana i mjeseca iskazane kao dvije cifre
	cout << datum19062019_1015.ToCharArray() << endl;//treba ispisati: 19/06/2019 10:15
	temp = datum05072019_1231;
	cout << temp.ToCharArray() << endl;//treba ispisati: 05/07/2019 12:31

	const int DictionaryTestSize = 9;
	Dictionary<int, int> Dictionary1;
	for (size_t i = 0; i < DictionaryTestSize; i++)
		Dictionary1.AddElement(i + 1, i*i);

	try {
		//vraca elemente kolekcije koji se nalaze na lokacijama definisanim vrijednostima parametara (npr. 2 - 7). 
		//funkcija baca izuzetak u slucaju da se zahtijeva lokacija koja ne postoji ili je vrijednost posljednje lokacije manja od pocetne
		Dictionary<int, int> opseg = Dictionary1.getRange(2, 7);
		cout << opseg << endl;
		Dictionary1.getRange(7, 11);
	}
	catch (exception& err) {
		cout << err.what() << endl;
	}
	cout << Dictionary1 << endl;

	Dictionary<int, int> Dictionary2 = Dictionary1;
	cout << Dictionary2 << crt;

	Dictionary<int, int> Dictionary3;
	Dictionary3 = Dictionary1;
	cout << Dictionary3 << crt;

	//napomena se moze dodati i prilikom kreiranja objekta
	Predmet MAT("Matematika", 7, "Ucesce na takmicenju"),
		UIT("Uvod u informacijske tehnologije", 9),
		RM("Racunarske mreze", 8),
		EN("Engleski jezik", 6);
	UIT.DodajNapomenu("Pohvala za ostvareni uspjeh");
	cout << MAT << endl;

	/*
	email adresa mora biti u formatu: text-text@ nakon cega slijedi neka od sljedecih domena: hotmail.com ili outlook.com ili fit.ba. Pod text se podrazumijeva bilo koje slovo, malo ili veliko.
	u slucaju da email adresa nije validna, postaviti je na defaultnu: notSet@fit.ba
	za provjeru koristiti regex
	*/
	Student jasmin("Jasmin Azemovic", "jasmin.azemovic@hotmail.com", "033 281 172");
	Student adel("Adel Handzic", "adel.handzic@fit.ba", "033 281 170");
	Student emailNotValid("Ime Prezime", "korisnik@lazna.ba", "033 281 170");

	cout << jasmin << adel << emailNotValid << endl;

	/*
	uspjeh se dodaje za svaki predmet na nivou godine studija.
	tom prilikom onemoguciti:
	- dodavanje istoimenih predmeta na nivou jedne godine,
	- dodavanje vise predmeta u kratkom vremenskom periodu (na nivou jedne godine, razmak izmedju dodavanja pojedinih predmeta mora biti najmanje 1 sat).
	godine (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za drugu godinu, pa onda za prvu godinu i sl.).
	Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	*/
	if (jasmin.AddPredmet(UIT, DRUGA, datum20062019_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(RM, DRUGA, datum30062019_1215))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(EN, PRVA, datum19062019_1015))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(MAT, PRVA, datum20062019_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	//ne treba dodati MAT jer je vec dodana u prvoj godini
	if (jasmin.AddPredmet(MAT, PRVA, datum05072019_1231))
		cout << "Predmet uspjesno dodan!" << crt;
	//ne treba dodati UIT jer nije prosao 1 sat od dodavanja posljednjeg predmeta
	if (jasmin.AddPredmet(UIT, PRVA, datum20062019_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	/*nakon evidentiranja uspjeha na bilo kojem predmetu tj. prilikom uspjesno izvrsene funkcije AddPredmet,
	Studentu se salje email sa sadrzajem:
	FROM:info@fit.ba
	TO: emailStudenta
	Postovani ime i prezime, evidentirali ste uspjeh za X godinu studija.
	Pozdrav.
	FIT Team.

	//ukoliko je, nakon dodavanja predmeta, prosjek na nivou te godine veci od 8.0 Studentu se, pored email-a,
	salje i SMS sa sadrzajem: "Svaka cast za uspjeh X.X ostvaren u X godini studija".
	//slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
	*/
	cout << "USPJEH ISPISATI KORISTEĆI OSTREAM_ITERATOR" << endl;
	cout << jasmin << endl;
	//vraca broj ponavljanja odredjene rijeci u napomenama, koristiti sregex_iterator
//	cout << "Rijec takmicenje se pojavljuje " << jasmin.BrojPonavljanjaRijeci("takmicenju") << " puta." << endl;
	cout << crt;

	//vraca niz predmeta koji su evidentiranih u periodu izmedju vrijednosti proslijedjenih parametara
	vector<Predmet> jasminUspjeh = jasmin(new DatumVrijeme(18, 06, 2019, 10, 15), new DatumVrijeme(21, 06, 2019, 10, 10));
	for (Predmet u : jasminUspjeh)
		cout << u << endl;
	cout << crt;

	Uspjeh * uspjeh_I_godina = jasmin["PRVA"];//vraca uspjeh Studenta ostvaren u prvoj godini studija
	if (uspjeh_I_godina != nullptr)
		cout << *uspjeh_I_godina << endl;

	cin.get();
	system("pause>0");
}