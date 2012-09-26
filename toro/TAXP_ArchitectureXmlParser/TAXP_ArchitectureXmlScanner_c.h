#ifndef TAXP_ArchitectureXmlScanner_c_h
#define TAXP_ArchitectureXmlScanner_c_h
/*
 * D L G L e x e r  C l a s s  D e f i n i t i o n
 *
 * Generated from: parser.dlg
 *
 * 1989-2001 by  Will Cohen, Terence Parr, and Hank Dietz
 * Purdue University Electrical Engineering
 * DLG Version 1.33MR33
 */


#include "DLexerBase.h"

class TAXP_ArchitectureXmlScanner_c : public DLGLexerBase {
public:
public:
	static const int MAX_MODE;
	static const int DfaStates;
	static const int START;
	static const int QUOTED_VALUE;
	typedef unsigned short DfaState;

	TAXP_ArchitectureXmlScanner_c(DLGInputStream *in,
		unsigned bufsize=2000)
		: DLGLexerBase(in, bufsize, 0)
	{
	;
	}
	void	  mode(int);
	ANTLRTokenType nextTokenType(void);
	void     advance(void);
protected:
	ANTLRTokenType act1();
	ANTLRTokenType act2();
	ANTLRTokenType act3();
	ANTLRTokenType act4();
	ANTLRTokenType act5();
	ANTLRTokenType act6();
	ANTLRTokenType act7();
	ANTLRTokenType act8();
	ANTLRTokenType act9();
	ANTLRTokenType act10();
	ANTLRTokenType act11();
	ANTLRTokenType act12();
	ANTLRTokenType act13();
	ANTLRTokenType act14();
	ANTLRTokenType act15();
	ANTLRTokenType act16();
	ANTLRTokenType act17();
	ANTLRTokenType act18();
	ANTLRTokenType act19();
	ANTLRTokenType act20();
	ANTLRTokenType act21();
	ANTLRTokenType act22();
	ANTLRTokenType act23();
	ANTLRTokenType act24();
	ANTLRTokenType act25();
	ANTLRTokenType act26();
	ANTLRTokenType act27();
	ANTLRTokenType act28();
	ANTLRTokenType act29();
	ANTLRTokenType act30();
	ANTLRTokenType act31();
	ANTLRTokenType act32();
	ANTLRTokenType act33();
	ANTLRTokenType act34();
	ANTLRTokenType act35();
	ANTLRTokenType act36();
	ANTLRTokenType act37();
	ANTLRTokenType act38();
	ANTLRTokenType act39();
	ANTLRTokenType act40();
	ANTLRTokenType act41();
	ANTLRTokenType act42();
	ANTLRTokenType act43();
	ANTLRTokenType act44();
	ANTLRTokenType act45();
	ANTLRTokenType act46();
	ANTLRTokenType act47();
	ANTLRTokenType act48();
	ANTLRTokenType act49();
	ANTLRTokenType act50();
	ANTLRTokenType act51();
	ANTLRTokenType act52();
	ANTLRTokenType act53();
	ANTLRTokenType act54();
	ANTLRTokenType act55();
	ANTLRTokenType act56();
	ANTLRTokenType act57();
	ANTLRTokenType act58();
	ANTLRTokenType act59();
	ANTLRTokenType act60();
	ANTLRTokenType act61();
	ANTLRTokenType act62();
	ANTLRTokenType act63();
	ANTLRTokenType act64();
	ANTLRTokenType act65();
	ANTLRTokenType act66();
	ANTLRTokenType act67();
	ANTLRTokenType act68();
	ANTLRTokenType act69();
	ANTLRTokenType act70();
	ANTLRTokenType act71();
	ANTLRTokenType act72();
	ANTLRTokenType act73();
	ANTLRTokenType act74();
	ANTLRTokenType act75();
	ANTLRTokenType act76();
	ANTLRTokenType act77();
	ANTLRTokenType act78();
	ANTLRTokenType act79();
	ANTLRTokenType act80();
	ANTLRTokenType act81();
	ANTLRTokenType act82();
	ANTLRTokenType act83();
	ANTLRTokenType act84();
	ANTLRTokenType act85();
	ANTLRTokenType act86();
	ANTLRTokenType act87();
	ANTLRTokenType act88();
	ANTLRTokenType act89();
	ANTLRTokenType act90();
	ANTLRTokenType act91();
	ANTLRTokenType act92();
	ANTLRTokenType act93();
	ANTLRTokenType act94();
	ANTLRTokenType act95();
	ANTLRTokenType act96();
	ANTLRTokenType act97();
	ANTLRTokenType act98();
	ANTLRTokenType act99();
	ANTLRTokenType act100();
	ANTLRTokenType act101();
	ANTLRTokenType act102();
	ANTLRTokenType act103();
	ANTLRTokenType act104();
	ANTLRTokenType act105();
	ANTLRTokenType act106();
	ANTLRTokenType act107();
	ANTLRTokenType act108();
	ANTLRTokenType act109();
	ANTLRTokenType act110();
	ANTLRTokenType act111();
	ANTLRTokenType act112();
	ANTLRTokenType act113();
	static DfaState st0[258];
	static DfaState st1[258];
	static DfaState st2[258];
	static DfaState st3[258];
	static DfaState st4[258];
	static DfaState st5[258];
	static DfaState st6[258];
	static DfaState st7[258];
	static DfaState st8[258];
	static DfaState st9[258];
	static DfaState st10[258];
	static DfaState st11[258];
	static DfaState st12[258];
	static DfaState st13[258];
	static DfaState st14[258];
	static DfaState st15[258];
	static DfaState st16[258];
	static DfaState st17[258];
	static DfaState st18[258];
	static DfaState st19[258];
	static DfaState st20[258];
	static DfaState st21[258];
	static DfaState st22[258];
	static DfaState st23[258];
	static DfaState st24[258];
	static DfaState st25[258];
	static DfaState st26[258];
	static DfaState st27[258];
	static DfaState st28[258];
	static DfaState st29[258];
	static DfaState st30[258];
	static DfaState st31[258];
	static DfaState st32[258];
	static DfaState st33[258];
	static DfaState st34[258];
	static DfaState st35[258];
	static DfaState st36[258];
	static DfaState st37[258];
	static DfaState st38[258];
	static DfaState st39[258];
	static DfaState st40[258];
	static DfaState st41[258];
	static DfaState st42[258];
	static DfaState st43[258];
	static DfaState st44[258];
	static DfaState st45[258];
	static DfaState st46[258];
	static DfaState st47[258];
	static DfaState st48[258];
	static DfaState st49[258];
	static DfaState st50[258];
	static DfaState st51[258];
	static DfaState st52[258];
	static DfaState st53[258];
	static DfaState st54[258];
	static DfaState st55[258];
	static DfaState st56[258];
	static DfaState st57[258];
	static DfaState st58[258];
	static DfaState st59[258];
	static DfaState st60[258];
	static DfaState st61[258];
	static DfaState st62[258];
	static DfaState st63[258];
	static DfaState st64[258];
	static DfaState st65[258];
	static DfaState st66[258];
	static DfaState st67[258];
	static DfaState st68[258];
	static DfaState st69[258];
	static DfaState st70[258];
	static DfaState st71[258];
	static DfaState st72[258];
	static DfaState st73[258];
	static DfaState st74[258];
	static DfaState st75[258];
	static DfaState st76[258];
	static DfaState st77[258];
	static DfaState st78[258];
	static DfaState st79[258];
	static DfaState st80[258];
	static DfaState st81[258];
	static DfaState st82[258];
	static DfaState st83[258];
	static DfaState st84[258];
	static DfaState st85[258];
	static DfaState st86[258];
	static DfaState st87[258];
	static DfaState st88[258];
	static DfaState st89[258];
	static DfaState st90[258];
	static DfaState st91[258];
	static DfaState st92[258];
	static DfaState st93[258];
	static DfaState st94[258];
	static DfaState st95[258];
	static DfaState st96[258];
	static DfaState st97[258];
	static DfaState st98[258];
	static DfaState st99[258];
	static DfaState st100[258];
	static DfaState st101[258];
	static DfaState st102[258];
	static DfaState st103[258];
	static DfaState st104[258];
	static DfaState st105[258];
	static DfaState st106[258];
	static DfaState st107[258];
	static DfaState st108[258];
	static DfaState st109[258];
	static DfaState st110[258];
	static DfaState st111[258];
	static DfaState st112[258];
	static DfaState st113[258];
	static DfaState st114[258];
	static DfaState st115[258];
	static DfaState st116[258];
	static DfaState st117[258];
	static DfaState st118[258];
	static DfaState st119[258];
	static DfaState st120[258];
	static DfaState st121[258];
	static DfaState st122[258];
	static DfaState st123[258];
	static DfaState st124[258];
	static DfaState st125[258];
	static DfaState st126[258];
	static DfaState st127[258];
	static DfaState st128[258];
	static DfaState st129[258];
	static DfaState st130[258];
	static DfaState st131[258];
	static DfaState st132[258];
	static DfaState st133[258];
	static DfaState st134[258];
	static DfaState st135[258];
	static DfaState st136[258];
	static DfaState st137[258];
	static DfaState st138[258];
	static DfaState st139[258];
	static DfaState st140[258];
	static DfaState st141[258];
	static DfaState st142[258];
	static DfaState st143[258];
	static DfaState st144[258];
	static DfaState st145[258];
	static DfaState st146[258];
	static DfaState st147[258];
	static DfaState st148[258];
	static DfaState st149[258];
	static DfaState st150[258];
	static DfaState st151[258];
	static DfaState st152[258];
	static DfaState st153[258];
	static DfaState st154[258];
	static DfaState st155[258];
	static DfaState st156[258];
	static DfaState st157[258];
	static DfaState st158[258];
	static DfaState st159[258];
	static DfaState st160[258];
	static DfaState st161[258];
	static DfaState st162[258];
	static DfaState st163[258];
	static DfaState st164[258];
	static DfaState st165[258];
	static DfaState st166[258];
	static DfaState st167[258];
	static DfaState st168[258];
	static DfaState st169[258];
	static DfaState st170[258];
	static DfaState st171[258];
	static DfaState st172[258];
	static DfaState st173[258];
	static DfaState st174[258];
	static DfaState st175[258];
	static DfaState st176[258];
	static DfaState st177[258];
	static DfaState st178[258];
	static DfaState st179[258];
	static DfaState st180[258];
	static DfaState st181[258];
	static DfaState st182[258];
	static DfaState st183[258];
	static DfaState st184[258];
	static DfaState st185[258];
	static DfaState st186[258];
	static DfaState st187[258];
	static DfaState st188[258];
	static DfaState st189[258];
	static DfaState st190[258];
	static DfaState st191[258];
	static DfaState st192[258];
	static DfaState st193[258];
	static DfaState st194[258];
	static DfaState st195[258];
	static DfaState st196[258];
	static DfaState st197[258];
	static DfaState st198[258];
	static DfaState st199[258];
	static DfaState st200[258];
	static DfaState st201[258];
	static DfaState st202[258];
	static DfaState st203[258];
	static DfaState st204[258];
	static DfaState st205[258];
	static DfaState st206[258];
	static DfaState st207[258];
	static DfaState st208[258];
	static DfaState st209[258];
	static DfaState st210[258];
	static DfaState st211[258];
	static DfaState st212[258];
	static DfaState st213[258];
	static DfaState st214[258];
	static DfaState st215[258];
	static DfaState st216[258];
	static DfaState st217[258];
	static DfaState st218[258];
	static DfaState st219[258];
	static DfaState st220[258];
	static DfaState st221[258];
	static DfaState st222[258];
	static DfaState st223[258];
	static DfaState st224[258];
	static DfaState st225[258];
	static DfaState st226[258];
	static DfaState st227[258];
	static DfaState st228[258];
	static DfaState st229[258];
	static DfaState st230[258];
	static DfaState st231[258];
	static DfaState st232[258];
	static DfaState st233[258];
	static DfaState st234[258];
	static DfaState st235[258];
	static DfaState st236[258];
	static DfaState st237[258];
	static DfaState st238[258];
	static DfaState st239[258];
	static DfaState st240[258];
	static DfaState st241[258];
	static DfaState st242[258];
	static DfaState st243[258];
	static DfaState st244[258];
	static DfaState st245[258];
	static DfaState st246[258];
	static DfaState st247[258];
	static DfaState st248[258];
	static DfaState st249[258];
	static DfaState st250[258];
	static DfaState st251[258];
	static DfaState st252[258];
	static DfaState st253[258];
	static DfaState st254[258];
	static DfaState st255[258];
	static DfaState st256[258];
	static DfaState st257[258];
	static DfaState st258[258];
	static DfaState st259[258];
	static DfaState st260[258];
	static DfaState st261[258];
	static DfaState st262[258];
	static DfaState st263[258];
	static DfaState st264[258];
	static DfaState st265[258];
	static DfaState st266[258];
	static DfaState st267[258];
	static DfaState st268[258];
	static DfaState st269[258];
	static DfaState st270[258];
	static DfaState st271[258];
	static DfaState st272[258];
	static DfaState st273[258];
	static DfaState st274[258];
	static DfaState st275[258];
	static DfaState st276[258];
	static DfaState st277[258];
	static DfaState st278[258];
	static DfaState st279[258];
	static DfaState st280[258];
	static DfaState st281[258];
	static DfaState st282[258];
	static DfaState st283[258];
	static DfaState st284[258];
	static DfaState st285[258];
	static DfaState st286[258];
	static DfaState st287[258];
	static DfaState st288[258];
	static DfaState st289[258];
	static DfaState st290[258];
	static DfaState st291[258];
	static DfaState st292[258];
	static DfaState st293[258];
	static DfaState st294[258];
	static DfaState st295[258];
	static DfaState st296[258];
	static DfaState st297[258];
	static DfaState st298[258];
	static DfaState st299[258];
	static DfaState st300[258];
	static DfaState st301[258];
	static DfaState st302[258];
	static DfaState st303[258];
	static DfaState st304[258];
	static DfaState st305[258];
	static DfaState st306[258];
	static DfaState st307[258];
	static DfaState st308[258];
	static DfaState st309[258];
	static DfaState st310[258];
	static DfaState st311[258];
	static DfaState st312[258];
	static DfaState st313[258];
	static DfaState st314[258];
	static DfaState st315[258];
	static DfaState st316[258];
	static DfaState st317[258];
	static DfaState st318[258];
	static DfaState st319[258];
	static DfaState st320[258];
	static DfaState st321[258];
	static DfaState st322[258];
	static DfaState st323[258];
	static DfaState st324[258];
	static DfaState st325[258];
	static DfaState st326[258];
	static DfaState st327[258];
	static DfaState st328[258];
	static DfaState st329[258];
	static DfaState st330[258];
	static DfaState st331[258];
	static DfaState st332[258];
	static DfaState st333[258];
	static DfaState st334[258];
	static DfaState st335[258];
	static DfaState st336[258];
	static DfaState st337[258];
	static DfaState st338[258];
	static DfaState st339[258];
	static DfaState st340[258];
	static DfaState st341[258];
	static DfaState st342[258];
	static DfaState st343[258];
	static DfaState st344[258];
	static DfaState st345[258];
	static DfaState st346[258];
	static DfaState st347[258];
	static DfaState st348[258];
	static DfaState st349[258];
	static DfaState st350[258];
	static DfaState st351[258];
	static DfaState st352[258];
	static DfaState st353[258];
	static DfaState st354[258];
	static DfaState st355[258];
	static DfaState st356[258];
	static DfaState st357[258];
	static DfaState st358[258];
	static DfaState st359[258];
	static DfaState st360[258];
	static DfaState st361[258];
	static DfaState st362[258];
	static DfaState st363[258];
	static DfaState st364[258];
	static DfaState st365[258];
	static DfaState st366[258];
	static DfaState st367[258];
	static DfaState st368[258];
	static DfaState st369[258];
	static DfaState st370[258];
	static DfaState st371[258];
	static DfaState st372[258];
	static DfaState st373[258];
	static DfaState st374[258];
	static DfaState st375[258];
	static DfaState st376[258];
	static DfaState st377[258];
	static DfaState st378[258];
	static DfaState st379[258];
	static DfaState st380[258];
	static DfaState st381[258];
	static DfaState st382[258];
	static DfaState st383[258];
	static DfaState st384[258];
	static DfaState st385[258];
	static DfaState st386[258];
	static DfaState st387[258];
	static DfaState st388[258];
	static DfaState st389[258];
	static DfaState st390[258];
	static DfaState st391[258];
	static DfaState st392[258];
	static DfaState st393[258];
	static DfaState st394[258];
	static DfaState st395[258];
	static DfaState st396[258];
	static DfaState st397[258];
	static DfaState st398[258];
	static DfaState st399[258];
	static DfaState st400[258];
	static DfaState st401[258];
	static DfaState st402[258];
	static DfaState st403[258];
	static DfaState st404[258];
	static DfaState st405[258];
	static DfaState st406[258];
	static DfaState st407[258];
	static DfaState st408[258];
	static DfaState st409[258];
	static DfaState st410[258];
	static DfaState st411[258];
	static DfaState st412[258];
	static DfaState st413[258];
	static DfaState st414[258];
	static DfaState st415[258];
	static DfaState st416[258];
	static DfaState st417[258];
	static DfaState st418[258];
	static DfaState st419[258];
	static DfaState st420[258];
	static DfaState st421[258];
	static DfaState st422[258];
	static DfaState st423[258];
	static DfaState st424[258];
	static DfaState st425[258];
	static DfaState st426[258];
	static DfaState st427[258];
	static DfaState st428[258];
	static DfaState st429[258];
	static DfaState st430[258];
	static DfaState st431[258];
	static DfaState st432[258];
	static DfaState st433[258];
	static DfaState st434[258];
	static DfaState st435[258];
	static DfaState st436[258];
	static DfaState st437[258];
	static DfaState st438[258];
	static DfaState st439[258];
	static DfaState st440[258];
	static DfaState st441[258];
	static DfaState st442[258];
	static DfaState st443[258];
	static DfaState st444[258];
	static DfaState st445[258];
	static DfaState st446[258];
	static DfaState st447[258];
	static DfaState st448[258];
	static DfaState st449[258];
	static DfaState st450[258];
	static DfaState st451[258];
	static DfaState st452[258];
	static DfaState st453[258];
	static DfaState st454[258];
	static DfaState st455[258];
	static DfaState st456[258];
	static DfaState st457[258];
	static DfaState st458[258];
	static DfaState st459[258];
	static DfaState st460[258];
	static DfaState st461[258];
	static DfaState st462[258];
	static DfaState st463[258];
	static DfaState st464[258];
	static DfaState st465[258];
	static DfaState st466[258];
	static DfaState st467[258];
	static DfaState st468[258];
	static DfaState st469[258];
	static DfaState st470[258];
	static DfaState st471[258];
	static DfaState st472[258];
	static DfaState st473[258];
	static DfaState st474[258];
	static DfaState st475[258];
	static DfaState st476[258];
	static DfaState st477[258];
	static DfaState st478[258];
	static DfaState st479[258];
	static DfaState st480[258];
	static DfaState st481[258];
	static DfaState st482[258];
	static DfaState st483[258];
	static DfaState st484[258];
	static DfaState st485[258];
	static DfaState st486[258];
	static DfaState st487[258];
	static DfaState st488[258];
	static DfaState st489[258];
	static DfaState st490[258];
	static DfaState st491[258];
	static DfaState st492[258];
	static DfaState st493[258];
	static DfaState st494[258];
	static DfaState st495[258];
	static DfaState st496[258];
	static DfaState st497[258];
	static DfaState st498[258];
	static DfaState st499[258];
	static DfaState st500[258];
	static DfaState st501[258];
	static DfaState st502[258];
	static DfaState st503[258];
	static DfaState st504[258];
	static DfaState st505[258];
	static DfaState st506[258];
	static DfaState st507[258];
	static DfaState st508[258];
	static DfaState st509[258];
	static DfaState st510[258];
	static DfaState st511[258];
	static DfaState st512[258];
	static DfaState st513[258];
	static DfaState st514[258];
	static DfaState st515[258];
	static DfaState st516[258];
	static DfaState st517[258];
	static DfaState st518[258];
	static DfaState st519[258];
	static DfaState st520[258];
	static DfaState st521[258];
	static DfaState st522[258];
	static DfaState st523[258];
	static DfaState st524[258];
	static DfaState st525[258];
	static DfaState st526[258];
	static DfaState st527[258];
	static DfaState st528[258];
	static DfaState st529[258];
	static DfaState st530[258];
	static DfaState st531[258];
	static DfaState st532[258];
	static DfaState st533[258];
	static DfaState st534[258];
	static DfaState st535[258];
	static DfaState st536[258];
	static DfaState st537[258];
	static DfaState st538[258];
	static DfaState st539[258];
	static DfaState st540[258];
	static DfaState st541[258];
	static DfaState *dfa[542];
	static DfaState dfa_base[];
	static unsigned char *b_class_no[];
	static DfaState accepts[543];
	static DLGChar alternatives[543];
	static ANTLRTokenType (TAXP_ArchitectureXmlScanner_c::*actions[114])();
	static unsigned char shift0[257];
	static unsigned char shift1[257];
	int ZZSHIFT(int c) { return 1+c; }
//
// 133MR1 Deprecated feature to allow inclusion of user-defined code in DLG class header
//
#ifdef DLGLexerIncludeFile
#include DLGLexerIncludeFile
#endif
};
typedef ANTLRTokenType (TAXP_ArchitectureXmlScanner_c::*PtrTAXP_ArchitectureXmlScanner_cMemberFunc)();
#endif
