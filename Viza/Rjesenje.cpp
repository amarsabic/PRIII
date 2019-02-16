#include<iostream>
#include<string>
#include<exception>
#include<sstream>
#include<regex>
#include<vector>
#include<iterator>
#include<thread>
#include<mutex>

using namespace std;
mutex cuvar;
char * AlocirajNizKaraktera(const char* sadrzaj) {
	if (sadrzaj == nullptr) return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

const char *crt = "\n-----------------------------------------------\n";

enum Status { ZAHTJEV_ZAPRIMLJEN, ODUSTAO, ODBIJENA, IZDATA, ISTEKLA, PONISTENA };
enum Sortiranje { RASTUCI, OPADAJUCI };
enum Drzava { SAD, AUSTRALIJA, ENGLESKA };

class Datum {
	int *_dan, *_mjesec, *_godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum &d) {
		_dan = new int(*d._dan);
		_mjesec = new int(*d._mjesec);
		_godina = new int(*d._godina);
	}
	Datum& operator=(const Datum &d) {
		if (this != &d) {
			*_dan = *d._dan;
			*_mjesec = *d._mjesec;
			*_godina = *d._godina;
		}
		return *this;
	}
	int GetGodina() const {
		return *_godina;
	}
	bool operator==(const Datum& d) {
		return (*_dan + *_mjesec * 30 + *_godina * 365) == (*d._dan + *d._mjesec * 30 + *d._godina * 365);
	}
	bool operator<(const Datum& d) {
		return (*_dan + *_mjesec * 30 + *_godina * 365) < (*d._dan + *d._mjesec * 30 + *d._godina * 365);
	}
	bool operator>(const Datum& d) {
		return (*_dan + *_mjesec * 30 + *_godina * 365) < (*d._dan + *d._mjesec * 30 + *d._godina * 365);
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	friend ostream& operator<< (ostream &COUT, Datum &obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
		return COUT;
	}

	Datum& operator++() { //???
		(*_dan)++;
		return *this;
	}
};

template<class T1, class T2, int max>
class FITKolekcija {
	T1 * _elementi1[max];
	T2 * _elementi2[max];
	int _trenutno;
public:
	FITKolekcija() {
		for (size_t i = 0; i < max; i++) {
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
		_trenutno = 0;
	}
	FITKolekcija(const FITKolekcija& original)
	{
		for (int i = 0; i < original._trenutno; i++)
		{
			_elementi1[i] = new T1(*original._elementi1[i]);
			_elementi2[i] = new T2(*original._elementi2[i]);
		}
		_trenutno = original._trenutno;
	}
	FITKolekcija& operator=(const FITKolekcija& original)
	{
		if (this != &original) {
			for (size_t i = 0; i < max; i++) {
				delete _elementi1[i]; _elementi1[i] = nullptr;
				delete _elementi2[i]; _elementi2[i] = nullptr;
			}
			for (int i = 0; i < original._trenutno; i++)
			{
				_elementi1[i] = new T1(*original._elementi1[i]);
				_elementi2[i] = new T2(*original._elementi2[i]);
			}
			_trenutno = original._trenutno;
		}
		return *this;
	}
	bool Dodaj(T1& el1, T2& el2) {
		if (_trenutno == max)
			throw exception("Prekoracenje opsega kolekcije!");
		_elementi1[_trenutno] = new T1(el1);
		_elementi2[_trenutno] = new T2(el2);
		_trenutno++;
		return true;
	}
	void Sortiraj(Sortiranje sort) {
		if (sort == Sortiranje::RASTUCI) {
			bool promjena = true;
			while (promjena) {
				promjena = false;
				for (int i = 0; i < _trenutno - 1; i++)
				{
					if (*_elementi1[i] > *_elementi1[i + 1]) {
						T1 temp1 = *_elementi1[i];
						T2 temp2 = *_elementi2[i];
						*_elementi1[i] = *_elementi1[i + 1];
						*_elementi2[i] = *_elementi2[i + 1];
						*_elementi1[i + 1] = temp1;
						*_elementi2[i + 1] = temp2;
						promjena = true;
					}
				}
			}
		}
		if (sort == Sortiranje::OPADAJUCI) {
			bool promjena = true;
			while (promjena) {
				promjena = false;
				for (int i = 0; i < _trenutno - 1; i++)
				{
					if (*_elementi1[i] < *_elementi1[i + 1]) {
						T1 temp1 = *_elementi1[i];
						T2 temp2 = *_elementi2[i];
						*_elementi1[i] = *_elementi1[i + 1];
						*_elementi2[i] = *_elementi2[i + 1];
						*_elementi1[i + 1] = temp1;
						*_elementi2[i + 1] = temp2;
						promjena = true;
					}
				}
			}
		}
	}
	~FITKolekcija() {
		for (size_t i = 0; i < max; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
	}
	T1 ** GetT1() { return _elementi1; }
	T2 ** GetT2() { return _elementi2; }
	int GetTrenutno() const { return _trenutno; }

	friend ostream& operator<< (ostream &COUT, FITKolekcija &obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << *obj._elementi1[i] << " " << *obj._elementi2[i] << endl;
		return COUT;
	}
};

class Viza {
protected:
	Drzava _drzava;
	Datum _vazenjeOD;
	Datum _vazenjeDO;
	char * _napomena;
	// string se odnosi na razlog promjene statusa, a moguće je samo 10 izmjena statusa
	FITKolekcija<Status, string, 10> _statusi;
public:
	Viza(Drzava drzava, Datum _OD, Datum _DO, char * napomena = nullptr) :_vazenjeDO(_DO), _vazenjeOD(_OD)
	{
		if (napomena != nullptr) {
			_napomena = AlocirajNizKaraktera(napomena);
		}
		else
			_napomena = nullptr;
		_drzava = drzava;
	}
	bool DodajStatus(Status status, string napomena) {
		return _statusi.Dodaj(status, napomena);
	}
	bool operator==(const Viza& v) {
		return _drzava == v.GetDrzava() && _vazenjeDO == v.GetVazenjeDO() && _vazenjeOD == v.GetVazenjeOD();
	}
	const char* SetNapomena(string novaNapomena) {
		return _napomena = AlocirajNizKaraktera(novaNapomena.c_str());
	}
	Viza(const Viza& v) :_statusi(v._statusi)
	{
		_drzava = v._drzava;
		_vazenjeOD = v._vazenjeOD;
		_vazenjeDO = v._vazenjeDO;
		_napomena = AlocirajNizKaraktera(v._napomena);
	}
	Viza& operator=(const Viza& v)
	{
		if (this != &v) {
			_drzava = v._drzava;
			_vazenjeOD = v._vazenjeOD;
			_vazenjeDO = v._vazenjeDO;
			delete[] _napomena;
			_napomena = AlocirajNizKaraktera(v._napomena);
			_statusi = v._statusi;
		}
		return *this;
	}
	~Viza() { delete[] _napomena; _napomena = nullptr; }
	Datum GetVazenjeOD()const { return _vazenjeOD; }
	Datum GetVazenjeDO()const { return _vazenjeDO; }
	char * GetNapomena() { return _napomena; }
	FITKolekcija<Status, string, 10> & GetStatuse() { return _statusi; }
	virtual void Info() = 0;
	Drzava GetDrzava() const {
		return _drzava;
	}

};

class PoslovnaViza :public Viza {
	string _kompanija; //naziv kompanije koja potvrđuje da posluje sa aplikantom
public:
	PoslovnaViza(Drzava drzava, Datum _OD, Datum _DO, string kompanija, char * napomena = nullptr) : Viza(drzava, _OD, _DO, napomena) {
		_kompanija = kompanija;
	}
	friend ostream & operator << (ostream & COUT, PoslovnaViza & obj) {
		COUT << obj._drzava << " " << obj._vazenjeOD << " " << obj._vazenjeDO << " " << obj._kompanija << endl;
		COUT << ((obj._napomena != nullptr) ? obj._napomena : "") << endl;
		COUT << "STATUSI -> " << obj._statusi << endl;
		return COUT;

	}
	void Info() {
		cout << *this;
	}
};

class TuristickaViza : public Viza {
	string _adresa; //adresa hotele ili drugog smještaja na kojoj će aplikant boraviti
public:
	TuristickaViza(Drzava drzava, Datum _OD, Datum _DO, string adresa, char * napomena = nullptr) : Viza(drzava, _OD, _DO, napomena) {
		_adresa = adresa;
	}
	friend ostream & operator << (ostream & COUT, TuristickaViza & obj) {
		COUT << obj._drzava << " " << obj._vazenjeOD << " " << obj._vazenjeDO << " " << obj._adresa << endl;
		COUT << ((obj._napomena != nullptr) ? obj._napomena : "") << endl;
		COUT << "STATUSI -> " << obj._statusi << endl;
		return COUT;
	}
	void Info() {
		cout << *this;
	}
};

class Aplikant {
	char * _imePrezime;
	Datum * _datumRodjenja;
	char * _brojPasosa;
	vector<Viza *> _vize;
public:
	Aplikant(const char * imePrezime, Datum datumRodjenja, const char * brPasosa) {
		_imePrezime = AlocirajNizKaraktera(imePrezime);
		_brojPasosa = AlocirajNizKaraktera(brPasosa);
		_datumRodjenja = new Datum(datumRodjenja);
	}
	~Aplikant() {
		delete[]_imePrezime; _imePrezime = nullptr;
		delete[]_brojPasosa; _brojPasosa = nullptr;
		delete _datumRodjenja; _datumRodjenja = nullptr;
		for (size_t i = 0; i < _vize.size(); i++)
		{
			delete _vize[i]; _vize[i] = nullptr;
		}
	}
	bool DodajVizu(Viza* viza) {
		if (viza->GetVazenjeOD().GetGodina() - _datumRodjenja->GetGodina() < 10)
			return false;
		for (int i = 0; i < _vize.size(); i++)
		{
			if (_vize[i]->GetDrzava() == viza->GetDrzava() && _vize[i]->GetVazenjeDO() > viza->GetVazenjeOD()) {
				for (int j = 0; j < _vize[i]->GetStatuse().GetTrenutno(); j++)
				{
					if (*_vize[i]->GetStatuse().GetT1()[j] != Status::IZDATA)
						return false;
				}
			}
			if (_vize[i]->GetDrzava() == Drzava::SAD && viza->GetDrzava() == Drzava::SAD) {
				for (int j = 0; j < _vize[i]->GetStatuse().GetTrenutno(); j++)
				{
					if (*_vize[i]->GetStatuse().GetT1()[j] == Status::PONISTENA)
						return false;
				}
			}
		}

		PoslovnaViza* poslovna = dynamic_cast<PoslovnaViza*>(viza);
		TuristickaViza* turisticka = dynamic_cast<TuristickaViza*>(viza);
		if (poslovna != NULL)
			_vize.push_back(new PoslovnaViza(*poslovna));
		else if (turisticka != NULL)
			_vize.push_back(new TuristickaViza(*turisticka));
		else
			return false;
	}
	bool DodajStatus(Viza* viza, Status status, string napomena = " ") {
		for (int i = 0; i < _vize.size(); i++)
		{
			if (*_vize[i] == *viza) {

				bool zahtjevZaprimljen = false;
				bool izdata = false;
				for (int j = 0; j < _vize[i]->GetStatuse().GetTrenutno(); j++)
				{
					if (*_vize[i]->GetStatuse().GetT1()[j] == Status::ZAHTJEV_ZAPRIMLJEN) {
						zahtjevZaprimljen = true;
					}
					if (*_vize[i]->GetStatuse().GetT1()[j] == Status::IZDATA) {
						izdata = true;
					}
				}

				if (status == Status::ZAHTJEV_ZAPRIMLJEN) {
					return _vize[i]->DodajStatus(status, napomena);

				}
				else if ((status == Status::ODUSTAO || status == Status::ODBIJENA || status == Status::IZDATA) && zahtjevZaprimljen) {
					return _vize[i]->DodajStatus(status, napomena);

				}
				else if ((status == Status::ISTEKLA || status == Status::PONISTENA) && izdata) {
					return _vize[i]->DodajStatus(status, napomena);

				}
			}
		}
		return false;
	}
	void PrintajVizu(Viza* viza) {
		for (int i = 0; i < _vize.size(); i++)
		{
			if (*viza == *_vize[i]) {
				thread t([](Viza *viza) {
					cuvar.lock();
					viza->Info();
					this_thread::sleep_for(chrono::seconds(5));
					cuvar.unlock();
				}, _vize[i]);
				t.join();
			}
		}
	}
	vector<Viza *> GetVizeByDrzava(Drzava drzava, Status status) {
		//VRAĆA PODATKE O SVIM VIZAMA KOJE SU IZDATE ZA ODREĐENU DRŽAVU, A KOJE SU IMALE ODREĐENI STATUS
		vector<Viza *> temp;
		for (int i = 0; i < _vize.size(); i++)
		{
			if (_vize[i]->GetDrzava() == drzava) {
				bool imaStatus = false;
				bool izdata = false;
				for (int j = 0; j < _vize[i]->GetStatuse().GetTrenutno(); j++)
				{
					if (*_vize[i]->GetStatuse().GetT1()[j] == status)
						imaStatus = true;
					if (*_vize[i]->GetStatuse().GetT1()[j] == Status::IZDATA)
						izdata = true;
				}
				if (imaStatus && izdata)
					temp.push_back(_vize[i]);
			}
		}
		return temp;
	}

	friend ostream & operator << (ostream & COUT, Aplikant & obj) {
		COUT << obj._imePrezime << " " << obj._datumRodjenja << " " << obj._brojPasosa << endl;
		COUT << "VIZE -> " << endl;
		for (int i = 0; i < obj._vize.size(); i++)
		{
			obj._vize[i]->Info();
		}
		return COUT;
	}

	vector<Viza * > GetVize() { return _vize; }
};

void main() {

	Datum danas(15, 2, 2017);
	Datum sutra(++danas); // inkrementiranjem se uvecava dan za 1
	Datum prekosutra;
	Datum dat15022017(15, 2, 2017), dat18082017(18, 8, 2017), dat11052018(11, 5, 2018), dat11122017(11, 12, 2017);


	prekosutra = danas;
	if (prekosutra == danas)
		cout << "Datumi su jednaki" << endl;
	cout << danas << endl << sutra << endl << prekosutra << crt;

	int v6 = 6, v13 = 13, v32 = 32, v63 = 63, v98 = 98, v109 = 109, v196 = 196;

	FITKolekcija<int, int, 7> brojevi;
	brojevi.Dodaj(v196, v6);
	brojevi.Dodaj(v13, v32);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v63, v13);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v196, v6);

	cout << brojevi << crt;
	//NA KOJI NAČIN ĆE SE VRŠITI SORTIRANJE PRVENSTVENO ZAVISI OD VRIJEDNOSTI PARAMETRA, A VRŠI SE NA OSNOVU VRIJEDNOSTI TIPA T1
	brojevi.Sortiraj(RASTUCI);
	cout << brojevi << crt;
	brojevi.Sortiraj(OPADAJUCI);
	cout << brojevi << crt;

	try
	{
		//U SLUČAJU PREKORAČENJA DEFINISANE VELIČINE KOLEKCIJE, POTREBNO JE BACITI IZUZETAK
		brojevi.Dodaj(v13, v196);
	}
	catch (exception & err)
	{
		cout << "Error -> " << err.what() << endl;
	}

	Aplikant jasmin("Jasmin Azemovic", Datum(15, 5, 1979), "BS6985645");
	PoslovnaViza poslovna(SAD, dat15022017, dat18082017, "Microsoft");

	/*
	NOVA VIZA SE NE MOŽE DODATI UKOLIKO:
	-	JE APLIKANT MLAĐI OD 10 GODINA
	-	U TOM PERIODU (ODNOSI SE NA PERIOD VIZE OD - DO) NE POSTOJI VAŽEĆA (POJAM VAŽEĆA PODRAZUMIJEVA
	DA JE POSLJEDNJI STATUS VIZE BIO IZDATA TJ. DA VIZA NIJE PONIŠTENA ILI ISTEKLA) VIZA ZA TU DRŽAVU, BEZ OBZIRA NA NJENU VRSTU.
	-	JE NEKA OD RANIJIH VIZA PONIŠTENA (OVO SE ODNOSI SAMO NA VIZE ZA DRŽAVU: SAD)
	*/
	if (jasmin.DodajVizu(&poslovna))
		cout << "Podaci o vizi uspješno zaprimljeni" << endl;
	/*
	STATUSI SE MOGU DODAVATI SAMO U REDOSLIJEDU U KOJEM SE NALAZE U ENUMERACIJI NPR.
	-	NIJE MOGUĆE ODBITI VIZU AKO PRETHODNO NIJE IMALA STATUS DA JE ZAHTJEV_ZAPRIMLJEN;
	-	NIJE MOGUĆE PONIŠTITI VIZU KOJA PRETHODNO NIJE IZDATA
	*/
	if (jasmin.DodajStatus(&poslovna, ZAHTJEV_ZAPRIMLJEN, "provjeriti navode sa predstavnicima kompanije Microsoft"))
		cout << "Status vize uspijesno promijenjen" << endl;
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;

	if (jasmin.DodajStatus(&poslovna, PONISTENA, "predstavnicima kompanije Microsoft nemaju informaciju o posjeti navedenog aplikanta"))
		cout << "Status vize uspijesno promijenjen" << endl;
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;

	TuristickaViza turisticka(ENGLESKA, dat18082017, dat11122017, "Whitehall Pl, London SW1A 2BD, UK");
	if (jasmin.DodajVizu(&turisticka))
		cout << "Podaci o vizi uspješno zaprimljeni" << endl;
	if (jasmin.DodajStatus(&turisticka, ZAHTJEV_ZAPRIMLJEN))
		cout << "Status vize uspijesno promijenjen" << endl;
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;

	if (jasmin.DodajStatus(&turisticka, IZDATA)) {
		cout << "Status vize uspijesno promijenjen" << endl;
		//U ZASEBNOJ NITI URADITI PRINTANJE VIZE...KORISTITI METODU SLEEP_FOR
		jasmin.PrintajVizu(&turisticka);
	}
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;

	//VRAĆA PODATKE O SVIM VIZAMA KOJE SU IZDATE ZA ODREĐENU DRŽAVU, A KOJE SU IMALE ODREĐENI STATUS
	vector<Viza *> jasminVize = jasmin.GetVizeByDrzava(SAD, PONISTENA);

	//UNUTAR NAPOMENE MIJENJA SADRŽAJ "nemaju informaciju" SA "su upoznti sa" - KORISTITI REGEX
	//jasmin.ZamijeniRijec("nemaju informaciju", "su upoznti sa");

	system("pause>null");
}