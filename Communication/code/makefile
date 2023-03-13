all: libs_g2r libs_train g2r train
g2r: bin/ressource_manager_stat bin/ressource_manager_dyn
train: bin/train_1_stat bin/train_1_dyn bin/train_3_stat bin/train_3_dyn bin/train_2_stat bin/train_2_dyn bin/train_4_stat bin/train_4_dyn
libs_g2r :lib/lib_g2r.a lib/lib_g2r.so
libs_train :lib/lib_train.a lib/lib_train.so

CPFLAGS=-I./include
#option -I <=> chemin des fichiers d'entetes inclus avec #include <>

LDFLAGS=-L./lib
#option -L <=> chemin des librairies non standards

#mise en place du mode verbose
#V = 1
ifeq ($V,1)
A=
else 
A=@
endif

#lib_g2r dyn et stat

build/lib_g2r.o : ./src/lib_g2r.c 
	@echo "On compile $< pour produire $@"
	$A gcc $(CPFLAGS) -c ./src/lib_g2r.c -o build/lib_g2r.o 

#production de la librairie du G2R statique
lib/lib_g2r.a : build/lib_g2r.o
	@echo "On compile $< pour produire la librairie du g2r statique $@"
	$A ar qvs lib/lib_g2r.a build/lib_g2r.o

#production de la librairie du G2R dynamique
build/lib_g2r_fPic.o : src/lib_g2r.c 
	$A gcc -fPIC -c src/lib_g2r.c -o build/lib_g2r_fPic.o

lib/lib_g2r.so : build/lib_g2r_fPic.o
	@echo "On compile $< pour produire la librairie du g2r dynamique $@"
	$A gcc -shared build/lib_g2r_fPic.o -o lib/lib_g2r.so

#lib_train dyn et stat

build/lib_train.o : ./src/lib_train.c 
	@echo "On compile $< pour produire $@"
	$A gcc $(CPFLAGS) -c ./src/lib_train.c -o build/lib_train.o 

#production de la librairie du train statique
lib/lib_train.a : build/lib_train.o
	@echo "On compile $< pour produire la librairie statique du train $@"
	$A ar qvs lib/lib_train.a build/lib_train.o

#production de la librairie du train dynamique
build/lib_train_fPic.o : src/lib_train.c 
	$A gcc -fPIC -c src/lib_train.c -o build/lib_train_fPic.o

lib/lib_train.so : build/lib_train_fPic.o
	@echo "On compile $< pour produire la librairie dynamique du train $@"
	$A gcc -shared build/lib_train_fPic.o -o lib/lib_train.so
	
	

# demander une édition de liens statique avec la lib lib_g2r : -l<NOM>
bin/ressource_manager_stat: examples/ressource_manager_2.c
	@echo "Edition de lien statique pour $< avec la librairie lib_g2r"
	$A gcc $(CPFLAGS) -static examples/ressource_manager_2.c $(LDFLAGS) -l:lib_g2r.a -o bin/ressource_manager_stat

# demander une édition de liens dynamique avec la lib lib_g2r : -l<NOM>
bin/ressource_manager_dyn: examples/ressource_manager_2.c
	@echo "Edition de lien dynamique pour $< avec la librairie julius"
	$A gcc $(CPFLAGS) $(LDFLAGS) examples/ressource_manager_2.c -o bin/ressource_manager_dyn -l:lib_g2r.so

# demander une édition de liens statique avec la lib lib_train : -l<NOM>
bin/train_1_stat: examples/train_1.c
	@echo "Edition de lien statique pour $< avec la librairie lib_train"
	$A gcc $(CPFLAGS) -static examples/train_1.c $(LDFLAGS) -l:lib_train.a -o bin/train_1_stat

bin/train_2_stat: examples/train_2.c
	@echo "Edition de lien statique pour $< avec la librairie lib_train"
	$A gcc $(CPFLAGS) -static examples/train_2.c $(LDFLAGS) -l:lib_train.a -o bin/train_2_stat

bin/train_3_stat: examples/train_3.c
	@echo "Edition de lien statique pour $< avec la librairie lib_train"
	$A gcc $(CPFLAGS) -static examples/train_3.c $(LDFLAGS) -l:lib_train.a -o bin/train_3_stat

bin/train_4_stat: examples/train_4.c
	@echo "Edition de lien statique pour $< avec la librairie lib_train"
	$A gcc $(CPFLAGS) -static examples/train_4.c $(LDFLAGS) -l:lib_train.a -o bin/train_4_stat


# demander une édition de liens dynamique avec la lib lib_train : -l<NOM>

bin/train_1_dyn: examples/train_1.c
	@echo "Edition de lien dynamique pour $< avec la librairie lib_train"
	$A gcc $(CPFLAGS) $(LDFLAGS) examples/train_1.c -o bin/train_1_dyn -l:lib_train.so

bin/train_2_dyn: examples/train_2.c
	@echo "Edition de lien dynamique pour $< avec la librairie lib_train"
	$A gcc $(CPFLAGS) $(LDFLAGS) examples/train_2.c -o bin/train_2_dyn -l:lib_train.so

bin/train_3_dyn: examples/train_3.c
	@echo "Edition de lien dynamique pour $< avec la librairie lib_train"
	$A gcc $(CPFLAGS) $(LDFLAGS) examples/train_3.c -o bin/train_3_dyn -l:lib_train.so

bin/train_4_dyn: examples/train_4.c
	@echo "Edition de lien dynamique pour $< avec la librairie lib_train"
	$A gcc $(CPFLAGS) $(LDFLAGS) examples/train_4.c -o bin/train_4_dyn -l:lib_train.so
	
	

clean: 
	rm -rf ./build/*.o ./bin/* *.o *.exe *.a *.sa
