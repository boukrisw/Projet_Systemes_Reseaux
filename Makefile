CC=gcc


.PHONY: clean all


%: %.o
			$(CC)  $^  -o  $@

%.o: %.c
	 		$(CC)  -c $^

clean:
	 		@echo "On efface les fichiers objets"
			rm client
			rm serveur
