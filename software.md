# Software


# Ovladani 
* Sachy jsou vybavene dvema klavesnicemi (pro kazdeho hrace jedna). Pomoci techto klávesnic lze ovladat sachy (zadavani tahu, prehravani tahu `BB`, vyklizeni sachovnice `AA` nebo `CC` - 2 verze vyklizeni sachovnice) i bez pripojeneho PC. Pokud je pri hre PC pripojene, po seriove lince se z Arduina posilaji aktualni informace o stavu hry.

* Samotne programovani probiha pres pripojeny PC. 

* V poslednich trech verzich programu lze komunikovat s Arduinem pres seriovou linku obousmerne. Podrobnosti o komunikaci se seriovou linkou (prikazy) se nachazeji nize.


# Seriova linka 
1. Vypsani stavu hry

* Prikaz `Stav?` vrati aktualni stav hry (tj. vypise aktualni souradnice vsech figurek).

2. Zadavani noveho tahu

* Prikazem `Novy tah ....` se ulozi aktualni tah do pameti pro prehrati. 

  Napriklad `Novy tah A2A3` => vychozi pozice `A2` a cilova pozice `A3`.

3. Prehravani partie (zadanych tahu)

* Prikazem `Prehrat partii` se spusti prehravani tahu ulozenych v pameti + vypise obsah pameti prehravanych tahu. 

4. Vyklizeni sachovnice

* Prikazem `Vyklidit sachovnici` se spusti vyklizeni sachovnice.



# Éra rozšiřování softwaru

* `sachy_puvodni_verze.ino`            - Verze sachu od D. Truhlare, puvodni soubor, ze ktereho vychazeji dalsi verze. 

* `sachy_kontrola_tahu.ino`            - Verze sachu s doplnenim kontroly tahu a zakladnich pravidel hry. 

* `sachy_pravidla_vyhazovani1.ino`     - Verze sachu s pravidly + doplneni rozpoznani vyhozene figurky. 

* `sachy_vyklizeni.ino`                - Verze sachu s pridanim zadavani 2 pismen - pro aktivaci vyklizeni.

* `sachy_vyklizeni3_uprava_planovace`  - Verze sachu s upravou struktury planovace (rozdeleni do vice funkcí).

* `sachy_vyklizeni4_pamatovani_tahu.ino` - Verze sachu s moznosti zapamatovani 1 tahu.

* `sachy_vyklizeni_prehrani_tahu`      - Verze sachu s moznosti prehrani 1 tahu.

* `sachy_vyklizeni5.ino`               - Verze sachu s moznosti zapamatovani libovolneho poctu tahu + jejich prehrati.

* `sachy_vyklizeni6.ino`               - Verze sachu s moznosti libovolnym prehravanim tahu + vyklizeni sachovnice

* `sachy_vyklizeni8.ino`               - Verze sachu s pridanim druhe (rychlejsi) moznosti vyklizeni sachovnice.

* `sachy_vyklizeni_cteni`              - Verze sachu s pridanim zakladniho cteni ze serioveho kanalu (vypisovani stavu hry).

* `sachy_vyklizeni_cteni2`             - Verze sachu s pridanim moznosti zadani novych tahu a jejich prehrati (ze serioveho kanalu).

* `sachy_vyklizeni_cteni3`             - Verze sachu s pridanim moznosti vyklizeni sachovnice (ze serioveho kanalu).


