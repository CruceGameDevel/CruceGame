Regulile jocului
=========

Dupa cum ati scris, cruce este un joc asemanator cu whist, cu cateva mici 
exceptii care vor fi evidentiate mai jos. Incep astfel descrierea jocului de 
cruce cu elementele sale:

# Pachetul de carti

Este format din 24 de carti diferite impartite astfel:

* __4 culori__ a caror denumire variaza in functie de zona in care veti juca 
cruce. Eu le stiu sub urmatoarea denumire: duba, verde(sau frunza), ghinda si
rosu(sau inima rosie). Nici una din aceste culori nu este momentan superioara
alteia.
* __6 valori__ in cadrul fiecarei culori: II(doiar), III(treiar), IV(patrar),
IX(nouar), X(zaca) si A(as)

# Masa de joc

Numarul de jucatori prezenti la masa de cruce nu este fix. Acest joc se poate
desfasura in 2, 3 sau 4 jucatori. In primele 2 cazuri, jucatorii sunt pe cont
propriu(singuri in __echipa__ vom spune, pentru a putea vorbi mai apoi de echipa
cand vom vorbi despre scor), iar in cazul jocului in 4, jucatorii pozitionati
diametral opus la masa formeaza echipe.

# Jocul

Un joc de cruce(este posibil ca pe viitor sa folosesc si denumirea de cruceas)
este format din mai multe __ture__ si se incheie cand una din echipele prezente
la masa de joc atinge un anumit scor. Scorul acesta este stabilit la inceputul
meciului si este de regula una dintre valorile: 11, 15 sau 21. Pentru incheierea
unui joc este necesar ca diferenta dintre jucatorul care a depasit limita de
puncte si urmatorul jucator(in ordinea descrescatoare a punctajelor) sa fie de
minim 2 puncte. In caz contrar se va continua jocul pana cand aceasta conditie
va fi indeplinita.

# Tura

O tura se desfasoara in urmatoarele etape:

* Impartirea cartilor in maini de catre dealer. In jocul de zi cu zi(cand
  jucatorii chiar stau la masa, exista posibilitatea ca un jucator sa taie sau
  sa bata cartile, lucru neesential in varianta pe calculator a jocului).

    1. In cazul jocului cu 4 jucatori(joc in 4 il voi numi de acum incolo) se
       vor imparti 6 carti fiecarei persoane/
    2. In cazul jocului in 3, fiecare persoana va primi 8 carti.
    3. In cazul jocului in 2, situatia este putin mai complicata: fiecare
       jucator va primi 8 carti, iar 8 carti vor fi plasate pe masa, fara a fi
       vazute de jucatori.

* Licitatia incepand de la primul jucator care a primit cartile, in sens
  trigonometric, catre dealer(astfel dealer-ul este ultimul). Vom explica in
  detaliu licitatia imediat.
* Dupa terminarea licitatiei se incepe jocul efectiv. Castigatorul licitatiei
  este cel care va alege tromful(mai multe detalii despre tromf imediat) si da
  prima carte(obligatoriu prima carte data va fi tromf)
* Dupa fiecare mana jucata, cel care castiga mana este cel care va incepe mana
  urmatoare dand o carte jos.
* Cand jucatorii raman fara carti(a se observa ca fata de alte jocuri, aici
  acest numar este fix pentru un numar fix de jucatori) se stabileste scorul si
  se acorda punctele.
* Se trece la urmatoarea tura pana cand jocul ia sfarsit.

# Licitatia

Fiecare jucator spune cate crede ca va face(stiu ca vi s-ar putea parea ca
lipseste ceva dupa "cate", dar nu lipseste nimic). Cel care zice cele mai multe
pana la final, castiga licitatia. Acum sa vedem ce inseamna sa faci: __pas, una,
doua, etc.__

Pentru fiecare punct este necesara acumularea a 33 de puncte. Astfel ca sa
faceti _una_ aveti nevoie de __33__ de puncte, pentru _doua_ - __66__ de puncte,
_trei_ - __99__, _patru_ - __132__, _cinci_ - __165__, _sase_ - __198__(desi
sunt destul de increzator ca nu veti face 6 vreodata)

In cazul in care echipa care a castigat licitatia nu strange un numar suficient
de puncte pentru a isi satisface pariul, la scor acestea se vor scadea. Voi da
cateva exemple pentru a clarifica totul:

Sa zicem ca jucam in 4, iar scorul pana la tura curenta este 4 - 8. Licitatia
este castigata de un membru al primei echipe care a zis doua. La finalul turei,
prima echipa reuseste sa acumuleze un total de numai 64 de puncte, iar cea de-a
doua echipa, restul de 56 de puncte. Scorul in urma rundei va fi 2 - 9.

Sa zicem ca ne continuam jocul in 4 lasat la scorul de 2 - 9 si acelasi jucator
al primei echipe castiga licitatia cu doua. La finalul turei, se numara punctele
si prima echipa a acumulat 102 puncte, iar cea de-a doua 18. Scorul va fi 5 - 9
(A se observa diferenta fata de whist unde daca nu faci exact cat ai licitat,
iei minus, si toti jucatorii au pariurile lor, neexistand un singur pariu pentru
fiecare tura).

# Scorul

In principal este destul de intuitiv:

* Doiarul valoreaza 2 puncte
* Treiarul valoreaza 3 puncte
* Patrarul, 4 puncte
* Nouarul, 0(da, ati citit bine ZERO) puncte
* Zaca, zece puncte
* Asul, 11 puncte

Mai exista si unele combinatii de carti care pot aduce puncte in plus. Daca un
jucator are in mana un treiar si un patrar la aceeasi culoare, acestea formeaza
un __anunt__. Acesta valoreaza 20 sau 40 de puncte. 40 de puncte este valoarea
anutului de tromf, iar toate celelalte anunturi valoreaza 20 de puncte. Pentru a
anunta un anunt este necesar ca jucatorul care il detine sa dea primul si sa
puna pe masa unul dintre cele doua elemente componente(ori treiarul ori patrarul)

# Mana

Inca nu am stabilit ce este lumea obligata sa dea jos.

Vom trata separat cazul jocului in doi. Momentan ne referim doar la jocurile in
trei sau in patru.

Cand un jucator pune jos o carte de valoare V si culoare C, toti ceilalti
jucatori sunt obligati sa dea o carte de culoare C. In cazul in care un jucator
nu are o carte de culoare C el este obligat sa dea tromf. In cazul in care nici
tromf nu are, el poate da orice culoare jos.

O alta regula este marirea. Consideram V cea mai mare valoare de pe masa la un
moment dat. Jucatorul care este la rand este obligat sa dea jos o carte cu
valoarea V' > V, in cazul in care detine o astfel de carte. In caz contrar, el
poate da orice. In cazul in care s-a jucat tromf, lucrurile se complica putin.

Sa presupunem ca sunt al treilea jucator care da. Primul jucator a dat un patrar
de duba(duba nu este tromf. Tromf este verde, sa zicem). Eu am in mana asul si
doiarul de duba. In mod normal as fi obligat sa dau asul. Totusi, al doilea
jucator nu are duba si este obligat sa dea cu tromf(verde) si pune jos treiarul
de verde. In acest moment regula maririi pentru cartile de duba inceteaza, iar
eu voi putea sa dau doiarul de duba. Este randul celui de-al patrulea jucator.
Nici el nu are duba in mana, dar are in mana doiarul si patrarul de verde(tromf),
deci este obligat conform regulii maririi sa ia mana, dand patrarul de verde.

O formulare mai intuitiva a acestei reguli ar suna asa: La fiecare moment,
jucatorul care este la rand, are obligatia, daca poate, sa dea o carte care sa
ii asigure luarea mainii(bineinteles, el considera doar cartile expuse pana la el).

# Cine ia mana?

In cazul in care nu s-a jucat tromf, mana este luata de persoana care a dat
cartea cu valoarea cea mai mare, avand culoarea primei carti jucate(degeaba dau
eu as de ghinda<ghinda nu e tromf in exemplul de fata>, daca prima carte jucata
a fost duba, ca nu iau mana).

In cazul in care s-a jucat tromf, jucatorul care a dat tromful de valoarea cea
mai mare castiga mana.

Cand o mana este castigata de o echipa, valoarea tuturor cartilor din acea mana
se aduna la punctajul echipei. A se mentiona ca fiecare tura este inceputa cu
punctaj 0 de toate echipele.

Cine ia mana este si cel care va incepe urmatoarea mana.

# Jocul in doi

In cadrul jocului in doi, atata timp cat sunt carti jos, obligatia de a da la
culoare sau de a mari cartea de jos nu se respecta. Dupa fiecare mana jucata,
fiecare jucator ia cate o carte de jos.

Cred ca am cam spus tot ce era de spus si e timpul sa identificam niste notiuni
cheie:
* Joc
* Masa de joc
* Jucator
* Scor
* Tura
* Mana(totalitatea cartilor pe care le are jucatorul in mana la un moment dat)
* Licitatie
* Mana(din nou mana, dar cu sens diferit. Aici ne referim la notiunea explicata
 anterior<cea cu cartile puse pe masa>. Cred ca le vom deosebi fara probleme din
 context)
* Tromf

Acum voi incerca un rezumat al celor de mai sus

La un joc de cruce se joaca atatea ture pana cand o echipa poate fi declarata
castigatoare. In cadrul fiecarei ture, un jucator primeste un anumit numar de
carti in mana, acest numar depinzand de numarul de jucatori aflati la masa de
joc. Dupa ce toti jucatorii si-au vazut cartile, are loc licitatia, castigata
de cel care liciteaza cele mai multe. Castigatorul licitatiei alege tromful si
este cel care incepe prima mana. Se joaca un numar fix de maini, numar egal cu
numarul cartilor primite de fiecare jucator la impartirea cartilor. La finalul
tuturor mainilor se numara punctele si se actualizeaza scorul.
