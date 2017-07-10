/**
 * \file ReseauInterurbain.cpp
 * \brief Implémentattion de la classe ReseauInterurbain.
 * \author Olivier Léveillé-Gauvin
 * \version 0.5
 * \date 2017-06-27
 *
 *  Travail pratique numéro 2
 *
 */
#include <sstream>
#include <fstream>
#include <algorithm>
#include "ReseauInterurbain.h"
#include "AssertionException.h"

namespace TP2
{

void ReseauInterurbain::chargerReseau(std::ifstream & fichierEntree)
{
	std::string buff;

	getline(fichierEntree, nomReseau);
	nomReseau.erase(0, 20); // Enlève: Reseau Interurbain:

	int nbVilles;

	fichierEntree >> nbVilles;
	getline(fichierEntree, buff); //villes

	unReseau.resize(nbVilles);

	getline(fichierEntree, buff); //Liste des villes

	size_t i = 0;

	while(getline(fichierEntree, buff) && buff != "Liste des trajets:")
	{
		unReseau.nommer(i, buff);
		i++;
	}

	while(getline(fichierEntree, buff))
	{
		std::string source = buff;
		getline(fichierEntree, buff);
		std::string destination = buff;

		getline(fichierEntree, buff);
		std::istringstream iss(buff);

		float duree;
		iss >> duree;

		float cout;
		iss >> cout;

		unReseau.ajouterArc(unReseau.getNumeroSommet(source), unReseau.getNumeroSommet(destination), duree, cout);
	}
}

/*
 * \brief Constructeur
 *
 * \param[in] nom, une chaine de caractères représentant le nom du réseau.
 * \param[in] nbVilles, le nombre de villes dans le réseau
 */
ReseauInterurbain::ReseauInterurbain(std::string nom, size_t nbVilles) : nomReseau(nom){
	unReseau = Graphe(nbVilles);
}

/*
 * \brief Destructeur
 */
ReseauInterurbain::~ReseauInterurbain() {
}

/*
 * \brief Changer le nombre de villes du réseau.
 *
 * \pre La nouvelle taille est plus grande que la taille actuelle.
 *
 * \param[in] nouvelleTaille, le nouveau nombre de villes.
 */
void ReseauInterurbain::resize(size_t nouvelleTaille) {
	ASSERTION(static_cast<int>(nouvelleTaille) > unReseau.getNombreSommets());
	unReseau.resize(nouvelleTaille);
}

/*
 * \brief Explore un sommet, met à jour le vecteur sommetsVisites et ajoute le sommet à la pile.
 *
 * \pre Le sommet existe.
 *
 * \param[in] sommet, un entier non-signé représentant le sommet à explorer.
 * \param[in] g, le graphe contenant le sommet à explorer.
 * \param[in] sommetsVisites, un vecteur de booléen (sommetsVisites[sommet]), true si déjà visité, false sinon
 * \param[in] pile, une pile d'entier non-signés contenant les indexes des sommets visités, en ordre de visite.
 */
void ReseauInterurbain::explore(unsigned int sommet, Graphe& g, std::vector<bool> & sommetsVisites, std::stack<unsigned int> & pile){
	ASSERTION(static_cast<int>(sommet) < g.getNombreSommets());

	sommetsVisites[sommet] = true;
	pile.push(sommet);

	for (auto voisin : g.listerSommetsAdjacents(sommet)) {
		if (!sommetsVisites[voisin]) explore(voisin, g, sommetsVisites, pile);
	}
}

/*
 * \brief Parcours en profondeur du réseau, pour tous les sommets en partant du premier.
 *
 * \param[in] g, un Graphe à parcourir
 * \param[out] pile, une pile d'entiers non-signés contenant les indexes des sommets visités, en ordre de visite.
 *
 * \pre Le réseau contient un nombre de sommet plus grand que 0.
 */
std::stack<unsigned int> ReseauInterurbain::parcoursProfondeur(Graphe& g) {
	ASSERTION(g.getNombreSommets() > 0);

	std::stack<unsigned int> pile;
	std::vector<bool> sommetsVisites(g.getNombreSommets(), false);

	for (auto i = 0 ; i < g.getNombreSommets() ; i++){
		if (!sommetsVisites[i]) explore(i, g, sommetsVisites, pile);
	}
	return pile;
}

/*
 * \brief Recherche les composantes fortement connexes du réseau en utilisant l'algorithme de Kosaraju.
 *
 * \pre Le réseau à évaluer a un nombre de sommets plus grand que 0.
 *
 * \param[out] composantesFortConnexes, un vecteur de sous-vecteurs de chaînes de caractères.
 *             Chaque sous-vecteur représente une composante fortement connexe et contient les noms des villes de cette composante.
 */
std::vector<std::vector<std::string> > ReseauInterurbain::algorithmeKosaraju() {
	ASSERTION(unReseau.getNombreSommets() > 0); //En pratique, pas nécessaire car déjà vérifié dans parcoursProfondeur()

	Graphe inverse = unReseau.inverse();

	std::vector<bool> sommetsVisites(inverse.getNombreSommets(), false);

	auto pile = parcoursProfondeur(inverse);
	std::vector<std::vector<std::string>> composantesFortConnexes;

	while (!pile.empty()){
		auto sommet = pile.top();
		pile.pop();
		if (!sommetsVisites[sommet]) {
			std::stack<unsigned int> resultat;
			explore(sommet, inverse, sommetsVisites, resultat);
			std::vector<std::string> composante;
			while (!resultat.empty()){
				composante.push_back(unReseau.getNomSommet(resultat.top()));
				resultat.pop();
			}
			composantesFortConnexes.push_back(composante);
		}
	}
	return composantesFortConnexes;
}
/*
 * \brief Recherche le chemin le plus court, selon la pondération choisie (durée/coût) entre deux villes.
 *
 * \pre Les sommets source et destination existent.
 * \pre Tous les arcs ont des pondérations (coût et durée) non-négatives
 *
 * \param[in] source, une chaîne de caractères représentant le nom du sommet(ville) source.
 * \param[in] destination, une chaîne de caractères représentant le nom du sommet(ville) destination.
 * \param[in] dureeCout, un booléen, true si la durée doit être utilisée comme pondération, false si le coût doit être utilisé comme pondération.
 * \param[out] chemin, le chemin le plus court entre la source et la destination.
 */
Chemin ReseauInterurbain::rechercheCheminDijkstra(const std::string& source,
		const std::string& destination, bool dureeCout) const {

	ASSERTION(static_cast<int>(unReseau.getNumeroSommet(source)) < unReseau.getNombreSommets() and
			static_cast<int>(unReseau.getNumeroSommet(destination)) < unReseau.getNombreSommets());

	ASSERTION(unReseau.arcsSontNonNegatifs());

	auto index_source = unReseau.getNumeroSommet(source);
	auto index_destination = unReseau.getNumeroSommet(destination);

	std::vector<float> distances(unReseau.getNombreSommets(), MAX);
	std::vector<unsigned int> predecesseurs(unReseau.getNombreSommets(), MAX);

	std::vector<bool> solutionnes(unReseau.getNombreSommets(), false);

	distances[index_source] = 0;

	for (auto i = 0 ; i < unReseau.getNombreSommets() ; i++) {
		float min = MAX;
		unsigned int prochain = 0;
		for (auto i = 0 ; i < unReseau.getNombreSommets() ; i++) {
			if (!solutionnes[i] and distances[i] < min) {
				min = distances[i];
				prochain = i;
			}
		}

		solutionnes[prochain] = true;
		for (auto voisin : unReseau.listerSommetsAdjacents(prochain)) {
			if (!solutionnes[voisin]) {
				float tmp;
				if (dureeCout) tmp = distances[prochain] + (unReseau.getPonderationsArc(prochain, voisin).duree);
				if (!dureeCout) tmp = distances[prochain] + (unReseau.getPonderationsArc(prochain, voisin).cout);
				if (tmp < distances[voisin]) {
					distances[voisin] = tmp;
					predecesseurs[voisin] = prochain;
				}
			}
		}
	}

	Chemin chemin;

	if (distances[index_destination] == MAX) chemin.reussi = false;

	else {
		chemin.reussi = true;
		if (dureeCout) chemin.dureeTotale = distances[index_destination];
		if (!dureeCout) chemin.coutTotal = distances[index_destination];
		auto sentinelle = index_destination;
		std::stack<std::string> villes;
		while (sentinelle != index_source) {
			villes.push(unReseau.getNomSommet(sentinelle));
			sentinelle = predecesseurs[sentinelle];
		}
		chemin.listeVilles.push_back(source);
		while (!villes.empty()) {
			chemin.listeVilles.push_back(villes.top());
			villes.pop();
		}
	}

	return chemin;
}

} //Fin du namespace

