Dezvoltarea unui joc de cărți
=======

##Introducere

Acest proiect este unul didactic, gratuit și open-source. Scopul său principal nu este 
produsul în sine, ci cunoștiințele acumulate în decursul dezvoltării sale.

Participarea la acest proiect da ocazia celor interesați să vadă cum e să dezvolți un 
produs software într-o echipă organizată, cum e să nu conteze doar executabilul rezultat, 
ci și codul în sine, și cum e să trebuiască să interacționezi cu ceva scris de o altă
persoană și să știi că ceea ce scrii la rândul tău va trebui fi înțeles și folosit și de 
altcineva.

Cei vizați sunt în principiu elevii și stundenții, care până acum n-au fost obișnuiți 
decât să transpună diverși algoritmi; însă oricine poate participa.

##De ce un joc de cărți?

Am ales dezvoltarea unui joc deoarece majoritatea am visat cândva, tineri programatori în 
devenire fiind, să dezvoltăm un joc.

Am ales dezvoltarea unui joc de cărți deoarece este simplu de realizat din punct de vedere
grafic.

Am ales jocul de *Cruce* pentru a promova ceva și de pe meleagurile noastre.

##Cunoștiințe necesare

Prima întrebare pe care mulți o vor adresa este: _Ce limbaj de programare vom folosi?_
Răspunsul la aceasta este: `C`.

Motivarea alegerii este foarte simplă:

* Foarte puțini sunt cei care nu s-au lovit încă de limbajul C.
* Limbajul este unul cross-platform, jocul dorindu-se a rula pe mai multe sisteme de 
operare.
* Limbajul C este mult mai simplu decât C++, astfel se evită complicații inutile.
* Efortul de programare în cazul acestui proiect este unul relativ redus, astfel, chiar 
dacă se va scrie în C, numărul liniilor de cod nu va fi foarte mare.
* API-ul majorității sistemelor de operare este accesibil din C, astfel jocul va avea o 
interfață modernă adecvată fiecărui sistem și se va putea juca și în rețea. NU, grafica în
C nu e limitată doar la linii și cerculețe specifice celor obișnuiți cu Borland C++ 3.1.

Din cadrul limbajului C, se poate spune că o persoană este pregătită să lucreze la acest 
proiect dacă:

* [Înțelege pointerii](http://www.cplusplus.com/doc/tutorial/pointers/)
* [Știe să apeleze compilatorul manual din consolă](http://forum.softpedia.com/topic/899129-lucruri-pe-care-orice-programator-ar-trebui-sa-le-tie/)
* Înțelege diferența dintre un fișier sursă (`.c`) și un header (`.h`)
* Știe să împartă codul unei aplicații în mai multe fișiere `.c`

##Diferențe față de scrierea programelor cu care sunteți obisnuiți

Dezvoltarea acestui joc poate apărea ca un șoc pentru cei obișnuiți doar să creeze un 
fișier `.c` în care trântesc niște cod apoi dau Run ... și merge.

În realitate lucrurile sunt mult mai simple decât par la prima vedere:

* Aplicația în sine va fi una modulară, compusă dintr-un executabil și una sau mai multe 
[biblioteci](http://forum.softpedia.com/topic/913043-misterele-dll-urilor/). Exemple de 
astfel de module ar fi: 
  * logica jocului: 
    * structuri de date specifice - `Player`, `Team`, `Round`
    * funcții de împărțire a cărților, de actualizare a scorului, ...
    * etc.
  * implementarea posibilității jocului în rețea/pe net
  * interfețe grafice pentru Linux, Windows, etc.
* Primul cod ce se scrie este pentru biblioteca de bază 
* Deoarece fiecare e obișnuit să scrie cod în stilul propriu, s-au definit [reguli privind 
stilul de codare](https://github.com/danij/CruceGame/blob/develop/docs/coding-standard.md), reguli ce trebuie respectate de toți cei interesați.
* Funcțiile din biblioteca de bază nu conțin apeluri spre funcții gen `printf`, `scanf`. 
De ce? Deoarece această biblioteca nu știe (și nici nu o interesează) cum sunt afișate 
datele, sau de unde sunt citite. Astfel funcțiile vor primi toate informațiile necesare 
prin intermediul parametriilor, ex. `struct Team *team_createTeam(char *name)`
* Deoarece o bibliotecă conține doar astfel de funcții, testarea acesteia sa va realiza 
folosind [unit tests](http://en.wikipedia.org/wiki/Unit_testing). Acestea sunt compuse 
din simple funcții care apelează funcțiile din bibliotecă cu anumiți parametrii, iar apoi
compară dacă rezultatul apelului este cel așteptat. Un exemplu de astfel de test (care
verifică dacă funcția `adună`, apelată cu parametrii `2` și `3`, returnează rezultatul `5`
) este: `verifica_adevarat(5, aduna(2,3));`
* În cadrul acestui proiect, testele sunt rulate folosind utilitarul 
[Cutter](http://cutter.sourceforge.net/)
* Compilarea codului nu se realizează apelând manual compilatorul (`gcc`), ci folosind 
un set de utilitare denumite `autotools`. Acestea prezintă avantajul că necesită doar o
ușoară configurare, iar apoi generează scripturi care se ocupă de tot, de la găsirea 
locației compilatorului, la compilarea codului și execuția testelor. 
* Cu toate că utilitarele `autotools` sunt la ele acasă sub Unix/Linux, voi scrie curând 
un tutorial despre cum pot fi folosite cu succes și de sub Windows, astfel încât doritorii
ce folosesc acest sistem de operare să nu se simtă descurajați.

Un aspect foarte important atunci când se lucrează în echipă este partajarea codului. 
Codul nu va fi trimis pur și simplu prin email de la o persoană la alta, ci se va folosi
aplicația `git` alături de serviciul gratuit de hosting `github`. Un tutorial fain se 
găseste la adresa: http://git-scm.com/book/en/Getting-Started iar informații specifice 
acestui proiect se găasesc în documenție.

##OK, sunt interesat, ce fac mai departe?

Primul pas care trebuie urmat e citirea în întregime a documentației proiectului. Aceasta
se găsește în folderul [docs/](https://github.com/danij/CruceGame/tree/develop/docs) și 
este actualizată în permanență. Documentația prezintă pașii următori și cuprinde inclusiv 
regulile jocului (pentru cei ce nu le știu).

Deoarece pot participa programatori din orice colț al lumii, limba oficială pentru 
dezvoltare și comunicare este __engleza__.