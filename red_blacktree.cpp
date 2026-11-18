#include <iostream>
#include <map>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>
using namespace std;
using namespace std::chrono;

struct Noeud {
    int valeur;
    bool rouge;
    Noeud* gauche;
    Noeud* droite;
    Noeud* parent;

    Noeud(int v) {
        valeur = v;
        rouge = true;
        gauche = nullptr;
        droite = nullptr;
        parent = nullptr;
    }
};

bool estRouge(Noeud* n) {
    return n != nullptr && n->rouge;
}

class ArbreRN {
private:
    Noeud* racine;

    void rotationGauche(Noeud* x) {
        Noeud* y = x->droite;
        x->droite = y->gauche;
        if (y->gauche != nullptr) {
            y->gauche->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            racine = y;
        } else if (x == x->parent->gauche) {
            x->parent->gauche = y;
        } else {
            x->parent->droite = y;
        }
        y->gauche = x;
        x->parent = y;
    }

    void rotationDroite(Noeud* x) {
        Noeud* y = x->gauche;
        x->gauche = y->droite;
        if (y->droite != nullptr) {
            y->droite->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            racine = y;
        } else if (x == x->parent->droite) {
            x->parent->droite = y;
        } else {
            x->parent->gauche = y;
        }
        y->droite = x;
        x->parent = y;
    }

    void corrigerInsertion(Noeud* n) {
        while (n != racine && estRouge(n->parent)) {
            Noeud* parent = n->parent;
            Noeud* grandParent = parent->parent;

            if (parent == grandParent->gauche) {
                Noeud* oncle = grandParent->droite;
                if (estRouge(oncle)) {
                    parent->rouge = false;
                    oncle->rouge = false;
                    grandParent->rouge = true;
                    n = grandParent;
                } else {
                    if (n == parent->droite) {
                        n = parent;
                        rotationGauche(n);
                        parent = n->parent;
                    }
                    parent->rouge = false;
                    grandParent->rouge = true;
                    rotationDroite(grandParent);
                }
            } else {
                Noeud* oncle = grandParent->gauche;
                if (estRouge(oncle)) {
                    parent->rouge = false;
                    oncle->rouge = false;
                    grandParent->rouge = true;
                    n = grandParent;
                } else {
                    if (n == parent->gauche) {
                        n = parent;
                        rotationDroite(n);
                        parent = n->parent;
                    }
                    parent->rouge = false;
                    grandParent->rouge = true;
                    rotationGauche(grandParent);
                }
            }
        }
        racine->rouge = false;
    }

    void corrigerSuppression(Noeud* x, Noeud* parent) {
        while (x != racine && !estRouge(x)) {
            if (x == parent->gauche) {
                Noeud* frere = parent->droite;

                if (estRouge(frere)) {
                    frere->rouge = false;
                    parent->rouge = true;
                    rotationGauche(parent);
                    frere = parent->droite;
                }

                if (!estRouge(frere->gauche) && !estRouge(frere->droite)) {
                    frere->rouge = true;
                    x = parent;
                    parent = x->parent;
                } else {
                    if (!estRouge(frere->droite)) {
                        frere->gauche->rouge = false;
                        frere->rouge = true;
                        rotationDroite(frere);
                        frere = parent->droite;
                    }
                    frere->rouge = parent->rouge;
                    parent->rouge = false;
                    frere->droite->rouge = false;
                    rotationGauche(parent);
                    x = racine;
                }
            } else {
                Noeud* frere = parent->gauche;

                if (estRouge(frere)) {
                    frere->rouge = false;
                    parent->rouge = true;
                    rotationDroite(parent);
                    frere = parent->gauche;
                }

                if (!estRouge(frere->gauche) && !estRouge(frere->droite)) {
                    frere->rouge = true;
                    x = parent;
                    parent = x->parent;
                } else {
                    if (!estRouge(frere->gauche)) {
                        frere->droite->rouge = false;
                        frere->rouge = true;
                        rotationGauche(frere);
                        frere = parent->gauche;
                    }
                    frere->rouge = parent->rouge;
                    parent->rouge = false;
                    frere->gauche->rouge = false;
                    rotationDroite(parent);
                    x = racine;
                }
            }
        }
        if (x != nullptr) {
            x->rouge = false;
        }
    }

    void detruire(Noeud* n) {
        if (n == nullptr) return;
        detruire(n->gauche);
        detruire(n->droite);
        delete n;
    }

    int verifier(Noeud* n) {
        if (n == nullptr) return 1;
        if (n->rouge && (estRouge(n->gauche) || estRouge(n->droite))) return -1;
        if (n->gauche != nullptr && n->gauche->valeur >= n->valeur) return -1;
        if (n->droite != nullptr && n->droite->valeur <= n->valeur) return -1;
        int hauteurGauche = verifier(n->gauche);
        int hauteurDroite = verifier(n->droite);
        if (hauteurGauche == -1 || hauteurDroite == -1) return -1;
        if (hauteurGauche != hauteurDroite) return -1;
        if (n->rouge) return hauteurGauche;
        return hauteurGauche + 1;
    }

public:
    ArbreRN() {
        racine = nullptr;
    }

    ~ArbreRN() {
        detruire(racine);
    }

    void inserer(int valeur) {
        Noeud* parent = nullptr;
        Noeud* courant = racine;

        while (courant != nullptr) {
            parent = courant;
            if (valeur == courant->valeur) return;
            if (valeur < courant->valeur) {
                courant = courant->gauche;
            } else {
                courant = courant->droite;
            }
        }

        Noeud* nouveau = new Noeud(valeur);
        nouveau->parent = parent;

        if (parent == nullptr) {
            racine = nouveau;
        } else if (valeur < parent->valeur) {
            parent->gauche = nouveau;
        } else {
            parent->droite = nouveau;
        }

        corrigerInsertion(nouveau);
    }

    bool chercher(int valeur) {
        Noeud* courant = racine;
        while (courant != nullptr) {
            if (valeur == courant->valeur) return true;
            if (valeur < courant->valeur) {
                courant = courant->gauche;
            } else {
                courant = courant->droite;
            }
        }
        return false;
    }

    void supprimer(int valeur) {
        Noeud* aSupprimer = racine;
        while (aSupprimer != nullptr && aSupprimer->valeur != valeur) {
            if (valeur < aSupprimer->valeur) {
                aSupprimer = aSupprimer->gauche;
            } else {
                aSupprimer = aSupprimer->droite;
            }
        }
        if (aSupprimer == nullptr) return;

        if (aSupprimer->gauche != nullptr && aSupprimer->droite != nullptr) {
            Noeud* successeur = aSupprimer->droite;
            while (successeur->gauche != nullptr) {
                successeur = successeur->gauche;
            }
            aSupprimer->valeur = successeur->valeur;
            aSupprimer = successeur;
        }

        Noeud* enfant;
        if (aSupprimer->gauche != nullptr) {
            enfant = aSupprimer->gauche;
        } else {
            enfant = aSupprimer->droite;
        }
        Noeud* parent = aSupprimer->parent;

        if (parent == nullptr) {
            racine = enfant;
        } else if (parent->gauche == aSupprimer) {
            parent->gauche = enfant;
        } else {
            parent->droite = enfant;
        }
        if (enfant != nullptr) {
            enfant->parent = parent;
        }

        bool etaitNoir = !aSupprimer->rouge;
        delete aSupprimer;

        if (etaitNoir) {
            if (estRouge(enfant)) {
                enfant->rouge = false;
            } else {
                corrigerSuppression(enfant, parent);
            }
        }
    }

    bool estValide() {
        if (estRouge(racine)) return false;
        return verifier(racine) != -1;
    }
};

template <typename Fonction>
double mesurer(Fonction fonction) {
    auto debut = high_resolution_clock::now();
    fonction();
    auto fin = high_resolution_clock::now();
    return duration<double, milli>(fin - debut).count();
}

void comparer(int N) {
    vector<int> valeurs(N);
    for (int i = 0; i < N; i++) {
        valeurs[i] = i;
    }
    mt19937 generateur(42);
    shuffle(valeurs.begin(), valeurs.end(), generateur);

    ArbreRN arbre;
    map<int, int> m;

    double insertionRN = mesurer([&] {
        for (int v : valeurs) arbre.inserer(v);
    });
    double insertionMap = mesurer([&] {
        for (int v : valeurs) m.insert({v, v});
    });

    bool valide = arbre.estValide();

    int trouvesRN = 0;
    int trouvesMap = 0;
    double rechercheRN = mesurer([&] {
        for (int v : valeurs) if (arbre.chercher(v)) trouvesRN++;
    });
    double rechercheMap = mesurer([&] {
        for (int v : valeurs) if (m.find(v) != m.end()) trouvesMap++;
    });

    double suppressionRN = mesurer([&] {
        for (int v : valeurs) arbre.supprimer(v);
    });
    double suppressionMap = mesurer([&] {
        for (int v : valeurs) m.erase(v);
    });

    cout << fixed << setprecision(4);
    cout << "\n===== N = " << N << " =====\n";
    cout << "Arbre valide apres insertion : " << (valide ? "oui" : "NON") << "\n";
    cout << "Elements trouves : RN=" << trouvesRN << "  map=" << trouvesMap << "\n";
    cout << left << setw(14) << "Operation"
         << setw(16) << "ArbreRN (ms)"
         << setw(16) << "std::map (ms)" << "\n";
    cout << string(46, '-') << "\n";
    cout << setw(14) << "Insertion"   << setw(16) << insertionRN  << setw(16) << insertionMap  << "\n";
    cout << setw(14) << "Recherche"   << setw(16) << rechercheRN << setw(16) << rechercheMap << "\n";
    cout << setw(14) << "Suppression" << setw(16) << suppressionRN << setw(16) << suppressionMap << "\n";
}

int main() {
    comparer(100);
    comparer(1000);
    comparer(10000);
    comparer(100000);
    return 0;
}