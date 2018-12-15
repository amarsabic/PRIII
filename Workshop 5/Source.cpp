#include<iostream>
using namespace std;

auto crt = "----------------------------------------------------\n";

char* AlocirajNizKaraktera(const char *sadrzaj)
{
	int vel = strlen(sadrzaj) + 1;
	char *temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

class Vrijeme
{
	int* _sati;
	int* _minute;
	int* _sekunde;

public:

	//Konstruktor
	Vrijeme(int sati, int minute, int sekunde = 0) {
		_sati = new int(sati);
		_minute = new int(minute);
		_sekunde = new int(sekunde);
	}

	//Defaultni konstruktor
	Vrijeme() : _sati(nullptr), _minute(nullptr), _sekunde(nullptr) {}

	//Konstruktor kopije
	Vrijeme(const Vrijeme& v) //Kontruktor kopije
	{
		_sati = new int(*v._sati);
		_minute = new int(*v._minute);
		_sekunde = new int(*v._sekunde);
	}

	//Operator dodjele
	Vrijeme& operator=(const Vrijeme& v)
	{
		if (this != &v) {
			if (_sati != nullptr) delete _sati;
			if (_minute != nullptr) delete _minute;
			if (_sekunde != nullptr) delete _sekunde;

			_sati = new int(*v._sati);
			_minute = new int(*v._minute);
			_sekunde = new int(*v._sekunde);
		}
		return *this;
	}

	//Destruktor
	~Vrijeme() {
		delete _sati; _sati = nullptr;
		delete _minute; _minute = nullptr;
		delete _sekunde; _sekunde = nullptr;
	}


	//Povecavanje sati
	void PovecajSate(int minute)
	{
		*_minute += minute;

		while (*_minute >= 60)
		{
			(*_sati)++;
			*_minute -= 60;
		}
		if (*_sati > 24) *_sati = 0;
	}

	//_vrijemePolijetanja + _kasnjenje 14:25 + 30

	Vrijeme operator+(int minute)
	{
		Vrijeme temp(*this); //pomocni niz
		*temp._minute += minute; //14:80

		while (*temp._minute >= 60)
		{
			(*temp._sati)++; //15:80
			*temp._minute -= 60;  // 15:20
		}

		if (*temp._sati == 24) *temp._sati = 0;

		return temp;
	}

	int PretvoriMinute() {
		return *_sati * 60 + *_minute;
	}

	bool operator<(const Vrijeme& desni) const {

		if (*this->_sati < *desni._sati)
			return true;
		if (*this->_sati == *desni._sati && *this->_minute < *desni._minute)
			return true;
		if (*this->_sati == *desni._sati && *this->_minute == *desni._minute && *this->_sekunde < *desni._sekunde)
			return true;
		return false;
	}

	bool operator>(const Vrijeme& desni) const {
		return !(*this < desni);
	}

	//Preklopiti operator << za ispis objekata tipa Vrijeme

	friend ostream& operator<<(ostream&, const Vrijeme&);

};

ostream& operator<<(ostream& cout, const Vrijeme& v)
{
	cout << *v._sati << ":" << *v._minute << ":" << *v._sekunde;

	return cout;
}




class Let
{
	static const int _oznakaSize = 10;
	char _oznaka[_oznakaSize];
	char* _odrediste;
	int _brIzlazneKapije;
	Vrijeme _vrijemePolijetanja; //sati i minute
	int _trajanje; //izraženo u minutama
	int _kasnjenje; //izraženo u minutama

public:
	//Potrebne konstruktor i destruktor funkcije

	//Konstruktor
	Let(const char * Oznaka, const char * Odrediste, int brojKapije, int Trajanje, Vrijeme vrijemePolijetanja = Vrijeme(0, 0))
		: _vrijemePolijetanja(vrijemePolijetanja)
	{
		strcpy_s(_oznaka, Oznaka);
		_odrediste = AlocirajNizKaraktera(Odrediste);
		_brIzlazneKapije = brojKapije;
		_trajanje = Trajanje;
		_kasnjenje = 0;
	}

	//Defaultni
	Let() : _vrijemePolijetanja(0, 0), _odrediste(nullptr), _brIzlazneKapije(0), _trajanje(0), _kasnjenje(0), _oznaka("")
	{

	}

	//Konstruktor Kopije
	Let(const Let& desni) : _vrijemePolijetanja(desni._vrijemePolijetanja)
	{
		strcpy_s(_oznaka, desni._oznaka);
		/*if (_odrediste != NULL)
		delete[] _odrediste; _odrediste = nullptr;*/
		_odrediste = AlocirajNizKaraktera(desni._odrediste);
		_brIzlazneKapije = desni._brIzlazneKapije;
		_trajanje = desni._trajanje;
		_kasnjenje = desni._kasnjenje;
	}

	//Operator dodjele Let
	Let& operator =(const Let& desni)
	{
		if (this != &desni) {
			if (_odrediste != NULL)
				delete[] _odrediste; _odrediste = nullptr;
			_vrijemePolijetanja = desni._vrijemePolijetanja;
			strcpy_s(_oznaka, desni._oznaka);
			_odrediste = AlocirajNizKaraktera(desni._odrediste);
			_brIzlazneKapije = desni._brIzlazneKapije;
			_trajanje = desni._trajanje;
			_kasnjenje = desni._kasnjenje;
		}
		return *this;
	}


	//Destruktor
	~Let() { delete[] _odrediste; _odrediste = nullptr; }


	char * GetOdrediste() {
		return _odrediste;
	}

	char * GetOznaka() {
		return _oznaka;
	}



	//Funkciju sa jednim parametrom koja postavlja informaciju o eventualnom kašnjenju na vrijednost zadanu parametrom.
	void AvionKasni(int minutaKasni)
	{
		_kasnjenje = minutaKasni;
	}

	/*Preklopiti unarni operator "!" putem kojeg je moguće saznati da li odgovarajući let kasni ili ne (vratiti logičku
	vrijednost "true" u slučaju kašnjenja, u suprotnom vratiti "false").*/
	bool operator!()
	{
		if (_kasnjenje > 0)
			return true;
		return false;
	}

	//Funkciju koja kao rezultat vraća trajanje leta.
	int GetTrajanje() const
	{
		return _trajanje + _kasnjenje;
	}


	/*Preklopiti operator + na način da omogućava sabiranje objekata tipa "Let" i cijelog broja, pri
	čemu se kao rezultat dobiva novi objekat tipa "Let", u kojem je vrijeme polijetanja pomjereno
	unaprijed za iznos određen drugim sabirkom (računato u minutama).
	Također preklopiti i operator "+=" koji osigurava da izraz oblika "X += Y uvijek ima isto značenje kao i izraz
	"X = X + Y" */

	Let operator+(int novoVrijeme)
	{
		Let novi(*this); //Konstruktor kopije
		novi._vrijemePolijetanja.PovecajSate(novoVrijeme);

		return novi;
	}

	Let& operator +=(int novoVrijeme)
	{
		this->_vrijemePolijetanja.PovecajSate(novoVrijeme);
		return *this;
	}



	/* Funkciju kojom je moguće saznati očekivano vrijeme polijetanja kada se uračuna iznos kašnjenja
	u odnosu na predviđeno vrijeme polijetanja (preklopiti operator + u klasi Vrijeme). */


	Vrijeme ocekivanoVrijemePolijetanja()
	{
		if (_kasnjenje == 0) {
			return _vrijemePolijetanja;
		}
		else {
			Vrijeme temp(_vrijemePolijetanja + _kasnjenje);
			return temp;
		}
	}

	//Funkciju koja vraća očekivano vrijeme slijetanja

	Vrijeme ocekivanoVrijemeSlijetanja()
	{
		Vrijeme temp(_vrijemePolijetanja + _kasnjenje + _trajanje);

		return temp;
	}


	/*Preklopiti operator "++" na način da pomjera vrijeme polaska za jedan sat unaprijed.
	  Potrebno je podržati i prefiksnu i postfiksnu verziju ovog operatora.*/

	  //Prvi nacin

	  //Vrijeme operator++() { //20:15 --> 21:15
	  //	Vrijeme novo(_vrijemePolijetanja + 60);

	  //	return novo;
	  //}

	  //Vrijeme operator++(int minuti) { 
	  //	Vrijeme novo(_vrijemePolijetanja + 60);

	  //	return novo;
	  //}

	  //Drugi nacin

	Let& operator++()
	{
		this->_vrijemePolijetanja.PovecajSate(60);
		return *this;
	}

	Let& operator++(int minute)
	{
		this->_vrijemePolijetanja.PovecajSate(60);
		return *this;
	}

	/*Preklopiti relacione operatore "<" i ">" koji ispituju koji let nastupa ranije, odnosno kasnije.
		Operator "<" vraća logičku vrijednost "true" ukoliko polijetanje leta sa lijeve strane nastupa
		prije polijetanje leta sa desne strane, a u suprotnom vraća logičku vrijednost "false".
		Analogno vrijedi za operator ">". Prilikom upoređivanja treba uzeti u obzir i očekivano vrijeme kašnjenja,
		a ne samo planirano vrijeme polijetanja.*/

	bool operator<(Let& desni) // 14:25 > 20:15
	{
		Vrijeme thisLet = (this->_vrijemePolijetanja + this->_kasnjenje);
		Vrijeme desniLet = (desni._vrijemePolijetanja + desni._kasnjenje);

		return thisLet < desniLet;
	}

	bool operator>(Let& desni)
	{
		Vrijeme thisLet = (this->_vrijemePolijetanja + this->_kasnjenje);
		Vrijeme desniLet = (desni._vrijemePolijetanja + desni._kasnjenje);

		return !(thisLet < desniLet);
	}


	/*Preklopiti operator "<<" koji treba da podrži ispis objekata tipa "Let" na ekran. U slučaju da
		se radi o polijetanju bez kašnjenja, ispis bi trebao da izgleda kako slijedi:

			JFK 156 Atalanta    12:50   19:30   5

		Podaci predstavljaju redom: oznaku leta, naziv odredišta, vrijeme polijetanja, očekivano
		vrijeme slijetanja i broj izlazne kapije.

		U slučaju da se radi o letu koji kasni, ispis bi trebao izgledati kako slijedi:

			ZGK 932 Zagreb    15:50 (Planirano 15:30, Kasni 20 min)*/



	friend ostream& operator<<(ostream&, Let&);

};

ostream& operator<<(ostream& cout, Let& let)
{
	cout << let._oznaka << "  " << let.GetOdrediste() << "\t" << let.ocekivanoVrijemePolijetanja() << "\t" << let.ocekivanoVrijemeSlijetanja();
	if (let._kasnjenje > 0) cout << "(Planirano " << let._vrijemePolijetanja << ", Kasni " << let._kasnjenje << ")" << endl;
	else cout << endl;
	return cout;
}


class RasporedLetova
{
	int _brojRegistrovanihLetova;
	const int _maksimalanBrojLetova;
	Let** _letovi;

public:
	RasporedLetova(int maksimalanBrojLetova) : _maksimalanBrojLetova(maksimalanBrojLetova),
		_letovi(new Let*[_maksimalanBrojLetova]), _brojRegistrovanihLetova(0)
	{
	}

	~RasporedLetova()
	{
		for (int i = 0; i < _brojRegistrovanihLetova; i++)
		{
			delete _letovi[i];
			_letovi[i] = nullptr;
		}

		delete[] _letovi;
		_letovi = nullptr;

		_brojRegistrovanihLetova = 0;
	}

	/*Preklopiti operator "+=" na način da registruje novi let u raspored. Raspored letova u svakom momentu treba biti sortiran
	  prema vremenu polijetanja. Voditi računa o maksimalnom broju letova.
	  Za potrebe poređenja vremena polaska letova preklopiti odgovarajuće operatore u klasi Vrijeme.*/

	void operator+=(const Let& let)
	{
		if (_brojRegistrovanihLetova == _maksimalanBrojLetova) {
			cout << "Dostigli se maksimalan broj letova!\n";
			return;
		}

		_letovi[_brojRegistrovanihLetova++] = new Let(let);

	}

	/*Preklopiti operator "-=" na način da ukljanja registrovani let sa oznakom definisanom putem desnog operanda.
	 Pri tome sačuvati redoslijed prethodno registrovanih letova. */

	void operator -=(const char* Oznaka)
	{
		int indeks = 0;

		for (int i = 0; i < _brojRegistrovanihLetova; i++)
		{
			if (strcmp(_letovi[i]->GetOznaka(), Oznaka) == 0) {
				indeks = i;
				break;
			}
		}

		if (indeks == 0) {
			cout << "Oznaka ne postoji!\n";
			return;
		}

		delete _letovi[indeks];

		for (int i = indeks; i < _brojRegistrovanihLetova; i++)
		{
			_letovi[i] = _letovi[i + 1];
		}

		_letovi[_brojRegistrovanihLetova - 1] = nullptr;
		_brojRegistrovanihLetova--;
	}

	//Preklopiti operator "[]" na način da vraća objekat tipa Let na osnovu proslijeđene pozicije u rasporedu (pozicije kreću od 1).

	Let * operator[](int pozicija)
	{
		pozicija--;

		if (pozicija<0 || pozicija>_brojRegistrovanihLetova)
			return nullptr;
		else
			return _letovi[pozicija];
	}

	/*Preklopiti operator "()" na način da letu sa definisanom oznakom pomjeri vrijeme polijetanja za vrijednost drugog parametra izraženog
	 u minutama. */

	void operator()(const char* Oznaka, int minute)
	{
		int pozicija = 0;

		for (int i = 0; i < _brojRegistrovanihLetova; i++)
		{
			if (strcmp(_letovi[i]->GetOznaka(), Oznaka) == 0)
			{
				pozicija = i;
				break;
			}
		}

		if (pozicija == 0) {
			cout << "Let ne postoji.\n";
			return;
		}

		*_letovi[pozicija] += minute;
	}

	//Funkciju koja daje prosječno trajanje svih pohranjenih letova.

	float GetProsjecnoTrajanjeLetova()
	{
		float sumaTrajanjaLetova = 0;
		int brojacTrenutnoLetova = 0;

		for (int i = 0; i < _brojRegistrovanihLetova; i++)
		{
			sumaTrajanjaLetova += _letovi[i]->GetTrajanje();
		}

		return sumaTrajanjaLetova / _brojRegistrovanihLetova;
	}

	/*Preklopiti operator "<<" koji ispisuje kompletan spisak svih letova, sortiran po očekivanim vremenima polazaka.
	  Za potrebe poređenja vremena polaska letova preklopiti odgovarajuće operatore u klasi Vrijeme. */

	friend ostream& operator<<(ostream& cout, const RasporedLetova& raspored)
	{
		cout << "Broj registrovanih letova--> " << raspored._brojRegistrovanihLetova << endl;
		cout << "Raspored letova: " << endl;
		for (int i = 0; i < raspored._brojRegistrovanihLetova; i++)
		{
			cout << *raspored._letovi[i] << endl;
		}
		return cout;
	}
};



int main()
{
	//Testiranje klase Vrijeme

	Vrijeme vrijeme1(14, 25);
	Vrijeme vrijeme2(20, 15);
	Vrijeme vrijeme3(20, 13);

	Vrijeme vrijeme4;
	vrijeme4 = vrijeme2;
	Vrijeme vrijeme5(vrijeme1);


	cout << crt;

	//Testiranje klase Nastava

	Let letA("ZG 125", "Zagreb-London", 1, 150, vrijeme1);
	Let letB("SA 230", "Sarajevo-Rim", 2, 180, vrijeme2);
	Let letC("MO 114", "Mostar-Minhen", 3, 180, vrijeme3);

	letA.AvionKasni(30); // _kasnjenje=30
	cout << ":::RASPORED LETOVA:::\n";
	cout << crt;
	cout << letA << crt;
	cout << letB << crt;
	cout << letC << crt;


	Let noviLetA = letA + 55; //Pomjeranje vremena polijetanja za 55 min
	cout << "Let pomjeren za 55 minuta; " << endl;
	cout << noviLetA;
	cout << crt;
	noviLetA += 10; // Pomjeranje za jos 10 minuta 
	cout << "Let pomjeren za jos 10 minuta" << endl;
	cout << noviLetA;

	cout << crt;

	cout << "Ocekivano vrijeme polijetanja za " << noviLetA.GetOdrediste() << " iznosi: ";
	cout << noviLetA.ocekivanoVrijemePolijetanja();
	cout << endl;
	cout << "Ocekivano vrijeme slijetanja za " << noviLetA.GetOdrediste() << " iznosi: ";
	cout << noviLetA.ocekivanoVrijemeSlijetanja();
	cout << endl;

	cout << crt << crt;

	++letB;
	cout << "Pomjeranje leta " << letB.GetOdrediste() << " za 1h. Novo vrijeme iznosi--> "; cout << letB.ocekivanoVrijemePolijetanja() << endl;
	letC++;
	cout << "Pomjeranje leta " << letC.GetOdrediste() << " za 1h. Novo vrijeme iznosi--> "; cout << letC.ocekivanoVrijemePolijetanja() << endl;

	cout << crt << crt;

	if (letA > letB)cout << "Prvi nastupa letB";
	else cout << "Prvi nastupa letA";

	//Testiranje klase RasporedLetova

	RasporedLetova raspored(5);
	raspored += letA;
	raspored += letB;
	raspored += letC;

	cout << raspored;
	cout << crt << crt;
	raspored -= "MO 114";
	cout << "Raspored nakon uklanjanja:" << endl;
	cout << raspored;
	cout << crt << crt;

	Let * temp = raspored[1];
	if (temp != NULL)
		cout << *temp;

	raspored("SA 230", 20); cout << endl;

	cout << "Prosjecno trajanje svih letova iznosi: " << raspored.GetProsjecnoTrajanjeLetova() << endl;
	cout << crt << crt;
	cout << raspored;


	system("pause>null");
	return 0;
}


