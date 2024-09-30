//
//  main.c
//  histogramme
//
//  Created by Léo Gallacio on 28/09/2024.
//

#include <stdio.h>
#include <stdlib.h>

#define nb_couleur 256

int stockage() {
    
    // 1) Lecture du fichier Pixmap.bin
    //      1.a) Lire les informations concernant la taille de l’image
    //      1.b) Valider les bornes, par. ex. taille négative!
    //      1.c) Reporter une erreur en cas de problème
    
    // 2) Lire les pixels de l’image et les stocker dans un tableau dynamique (malloc)
    //      2.c) • Lire tous les pixels en une fois et les stocker dans un tableau dynamique Pixmap
    //      2.b) • Valider la bonne lecture des pixels et reporter une erreur en cas de problème (ex. pas assez de pixels)
    
    // Ouvrir le fichier .bin
    FILE *file = fopen("/Users/leogallacio/Downloads/Pixmap.bin", "rb");

    // Créer tableau pour stocker les dimensions
    unsigned short dimensions[3];
    
    // Lire les 3 premières valeurs (dimensions)
    // 1.a) OK
    size_t dim_read = fread(dimensions, sizeof(unsigned short), 3, file);
    printf("Les dimensions sont : %hd, %hd, %hd\n\n", dimensions[0], dimensions[1], dimensions[2]);
    if (dim_read != 3) {
        printf("Erreur: Impossible de lire les dimensions.\n");
        fclose(file);
        return -1;
    }

    // Vérifier que les dimensions sont positives
    // 1.b) OK
    // 1.c) OK
    for (int i = 0; i < 3; i++) {
        if (dimensions[i] <= 0) {
            printf("Erreur: Dimension invalide.\n");
            fclose(file);
            return -1;
        }
    }

    // Calcul le nombre de pixel dans l'image
    int nb_pixels = dimensions[1] * dimensions[2]; // dimensions[1] = largeur, dimensions[2] = hauteur
    printf("Nombre de pixel : %d\n\n", nb_pixels);

    // Allouer de la mémoire pour les pixels
    // 2.a) OK
    unsigned char *pixmap = (unsigned char *)malloc(nb_pixels * sizeof(unsigned char));
    
    if (!pixmap) {
        printf("Erreur: Impossible d'allouer de la mémoire.\n");
        fclose(file);
        return -1;
    }

    // Lire les pixels du fichier
    // 2.b) OK
    size_t pix_read = fread(pixmap, sizeof(unsigned char), nb_pixels, file);
    for (int i=0; i < nb_pixels; i++){
        if (pixmap[i] != 0){
            printf("Pixel %d :\n", i);
            printf("    Décimale : %d", pixmap[i]); // Affichage décimale
            printf("    Hexadécimale : %x", pixmap[i]); // Affichage hexadécimal
            printf("    Binaire : ");
            for (int j=7; j>=0; j--){
                printf("%d", (pixmap[i]>>j) & 1); // Affichage binaire : conversion si pixmap[i] est 200 => 11001000 en binaire donc comparer les 8 bits avec 1 => pixmap[i]>>j & 1
            }
            printf("\n");
        }
    }
    if (pix_read != nb_pixels) {
        printf("Erreur: Nombre incompatible de pixels.\n");
        free(pixmap);
        fclose(file);
        return -1;
    }
    
    // Création de l'histogramme
    // Il y a 256 couleurs
    // Compter le nombre de pixels avec cette couleur
    unsigned char *histogramme = (unsigned char *)malloc(nb_couleur * sizeof(unsigned char));
    for (int k=0; k < nb_couleur; k++){
        for (int l=0; l < nb_pixels; l++){
            if (pixmap[l] == k){
                histogramme[k]++;
            }
        }
    }
    
    // GERER LES CONDITIONS EX : GARDER LES 5 PLUS GRANDES TRACES 
    
    printf("\n");
    for (int m=0; m < nb_couleur; m++){
        if (histogramme[m] != 0){
            printf("Couleur %d = %d\n", m, histogramme[m]);
        }
    }
    
    printf("\n");
    int trace=0;
    int couleur=0;
    for (int m=0; m < nb_couleur; m++){
        if (histogramme[m] == 4){
            printf("4 Points de contrôle : couleur %d\n", m);
        }
        if ((50 <= histogramme[m]) && (300 >= histogramme[m])){
            trace++;
        }
    }
    printf("%d Traces : ", trace);
    for (int m=0; m < nb_couleur; m++){
        if ((50 <= histogramme[m]) && (300 >= histogramme[m])){
            printf("couleur %d  ", m);
        }
    }
    
    int variable=0;
    printf("\n\n");
    
    // Fermeture du fichier
    fclose(file);

    // Libérer la mémoire après utilisation
    free(pixmap);

    return 1;
}

int main(){
    stockage();
    return 0;
}
