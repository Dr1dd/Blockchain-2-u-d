# Blockchain 2 užduotis:

Programos paleidimui jums reikės:

Clion ar kito IDE.


## v0.1 Programos versija:

[Reikalavimai:](https://github.com/blockchain-group/Blockchain-technologijos/blob/master/pratybos/2uzduotis-Blockchain.md#reikalavimai-versijai-v01-terminas-2019-10-18)

Pirma versija padaryta pagal užduoties reikalavimus (arba bent taip manoma).

Kaip naudotis programa? Tiesiog atsisiųskite failus ir sukompiliuokite projektą. 

Paleidus projektą bus automatiškai sugeneruojami 1000 ```User'ių``` ir 10000 ```Transactions```.

Taip pat atsitiktinai parenkamos 100 transakcijų, kurios sudarys naujo bloko ```body```.

Sudėjus visus reikiamus duomenis į naują bloką, jis pradedamas kasti ( ```mininti```). Tai tęsiasi iki tol, kol visos buvusios transakcijos nėra blokuose.

Pradinis bloko ```string'as```: 

```
    MainBlockHash = myBlock.getPreviousBlock()+std::to_string(myBlock.getTimestamp())+myBlock.getDifficultyTarget()+myBlock.getMerkleHash()+ myBlock.getVersion();
```
Visi bloke esantys duomenys yra sudedami į vieną ilgą string tipo kintamąjį.

Tada eina ciklas, kuris kiekvieną kartą prideda prie bloko string tipo kintamojo, kintamąjį ```Nonce```.

```
    do{
        TempBlockHash = MainBlockHash +std::to_string(Nonce);
        Hashish(TempBlockHash);
        Nonce++;
    }while(TempBlockHash>myBlock.getDifficultyTarget() && Nonce <=maxNonce);
    if(TempBlockHash<myBlock.getDifficultyTarget()) {
        myBlock.setCurrentBlock(TempBlockHash);
        myBlock.setNonce(Nonce-1);
    }
```

Jeigu gautasis hash'as yra mažesnis nei difficulty target - blokas yra skaitomas kaip "išminintas"

## v0.2 Programos versija:
Antra Blockchain užduoties versija, kurios reikalavimai: [link](https://github.com/blockchain-group/Blockchain-technologijos/blob/master/pratybos/2uzduotis-Blockchain.md#reikalavimai-versijai-v02-preliminarus-terminas-2019-10-25)

### Balanso tikrinimas: 

Pirmiausia tikrinima tuo metu, kai yra generuojamos transakcijos:

```
 while(coin > userVector[randomsk].balance) coin = dist(gen);
```

Tada tikrinama prieš keliant transakciją į bloką:
```
if(myUser[myTransactionsVectorCopy[tempTxval].getSendersID()].balance- myTransactionsVectorCopy[tempTxval].getValue()) >=0) tempTransactionBlock.push_back(myTransactionsVectorCopy[tempTxval]);
```
```.getSendersID``` yra siuntėjo id ```myUser``` vektoriuje.

### Transakcijos hash'o tikrinimas:

Prieš įpush'inant transakciją į transakcijų vektorių, kuris įeis į naują bloką, yra daromas transakcijos hash'o patikrinimas:

```
tempTxHashCheck = myTransactionsVectorCopy[tempTxval].getSender()+myTransactionsVectorCopy[tempTxval].getReceiver()+std::to_string(myTransactionsVectorCopy[tempTxval].getValue()) +std::to_string(myTransactionsVectorCopy[tempTxval].getDate());
            Hashish(tempTxHashCheck); // Duomenys užhashinami
            if(tempTxHashCheck == myTransactionsVectorCopy[tempTxval].getHash()) tempTransactionBlock.push_back(myTransactionsVectorCopy[tempTxval]); //Tikrinama ar gautasis Hash sutampa su transakcijų pool'e esančiu hash.
```
Į naują kintamąjį yra sudedami visi transakcijoje esantys duomenys ir jie yra tikrinami su ta pačia transakcija, esančia transakcijų pool'e. Jeigu viskas sutampa - transakcija yra įpush'inama į vektorių ```tempTransactionBlock```, kuris įeis į naujo bloko ```body```.


### Blokų kasimo tobulinimas:

Užduotis reikalavo vienu metu kasti ne 1 bloką, o 5. Šioje programoje tai buvo realizuota taip: 

Sugeneruojami 5 blokai, bet jie dar nėra kasami. Blokus sudaro: 100 Transakcijų, bloko versija, Merkle Root hash, Difficulty Target, buvusio bloko hash'as ir dabartinė data.

Transakcijos bloke yra parenkamos atsitiktinai iš transakcijų pool'o.

Vyksta normalus "mininimas", kuris tęsiasi iki tol, kol "mininamo" bloko ```Nonce``` nepasiekia tam tikrą skaičių (pvz.: iš pradžių tas skaičius yra 100000, bet jeigu visi "mininami" blokai neranda tinkamo hash'o blokui, tai tas skaičius yra padvigubinamas: 100k->200k->400k ir t.t.).

Užduotis reikalavo atsitiktinai parinkti, kuris blokas bus atsitiktinai "mininamas":

Iš pradžių yra sukuriami du vektoriai, iš kurių vienas bus statinis, o kitas bus dinaminis (vieno reikšmės keisis, o kito - ne):
```
     std::vector<int> arrayOfRandomBlockIndexes{0, 1, 2, 3, 4};
        std::vector<int> arrayCopy = arrayOfRandomBlockIndexes;
```
```
 minedBlock = mineBlock(blockCandidates[arrayCopy[randomBlock]], maxNonce, Nonce);
 arrayCopy.erase(arrayCopy.begin() + randomBlock);
...........................................
randomBlock = distribution(generation);
```
Jeigu kasto bloko Nonce yra 0 - tai reiškia, jog tinkamas Bloko Hash'as nebuvo rastas.

Jeigu kasant 5 blokus nebuvo rastas nei vienas tinkamas hash:

```
     Nonce = maxNonce;
      maxNonce *= 2;
```
Dabartinis Nonce yra padidinamas į maxNonce, o maxNonce yra padauginamas 2 kartus.

Pvz.: Nonce 0->100000, maxNonce 100000->200000




