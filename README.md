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
