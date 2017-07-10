/**
 * \file Graphe.cpp
 * \brief Implémentation d'un graphe orienté.
 * \author Olivier Léveillé-Gauvin
 * \version 0.5
 * \date 2017-06-27
 *
 *  Travail pratique numéro 2
 *
 */

#include "Graphe.h"
#include "AssertionException.h"
//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP2
{
/*
 * \brief Constructeur
 *
 * \param[in] n, le nombre de sommets du graphe.
 */
Graphe::Graphe(size_t n) : noms(std::vector<std::string>(n)), listesAdj(std::vector<std::list<Arc>>(n)), nbSommets(n), nbArcs(0){

}

/*
 * \brief Destructeur
 *
 */
Graphe::~Graphe() {
}

/*
 * \brief Retourne le nombre de sommets.
 *
 * \param[out] Le nombre de sommets.
 */
int Graphe::getNombreSommets() const {
	return nbSommets;
}

/*
 * \brief Retourne le nombre d'arcs.
 *
 * \param[out] Le nombre d'arcs.
 */
int Graphe::getNombreArcs() const {
	return nbArcs;
}

/*
 * \brief Change le nombre de sommets du graphe
 *
 * \param[in] nouvelleTaille, le nouveau nombre de sommets.
 *
 * \pre La nouveau nombre de sommets est plus grand que la taille actuelle.
 */
void Graphe::resize(size_t nouvelleTaille) {
	ASSERTION(nouvelleTaille > nbSommets);
	noms.resize(nouvelleTaille);
	listesAdj.resize(nouvelleTaille);
	nbSommets = nouvelleTaille;
}

/*
 * \brief Nomme un sommet.
 *
 * \param[in] sommet, l'index du sommet.
 * \param[in] nom, le nom à donner.
 *
 * \pre Le sommet passé en paramètre est plus petit que le nombre de sommets.
 */
void Graphe::nommer(size_t sommet, const std::string& nom) {
	ASSERTION(static_cast<int>(sommet) < getNombreSommets());
	noms[sommet].assign(nom);
}

/*
 * \brief Retourne le nom donné à un sommet.
 *
 * \param[in] sommet, l'index du sommet.
 * \param[out] Le nom du sommet.
 *
 * \pre Le sommet passé en paramètre est petit que le nombre de sommets.
 */
std::string Graphe::getNomSommet(size_t sommet) const {
	ASSERTION(static_cast<int>(sommet) < getNombreSommets());
	return noms[sommet];
}

/*
 * \brief Retourne l'index d'un sommet selon son nom.
 *
 * \param[in] nom, le nom du sommet recherché.
 * \param[out] Un entier représentant l'index du sommet recherché.
 *
 * \pre Le nombre de sommets est plus grand que 0.
 */
size_t Graphe::getNumeroSommet(const std::string& nom) const {
	ASSERTION(getNombreSommets() > 0);
	for (auto i = 0 ; i < getNombreSommets() ; i++){
		if (noms[i] == nom) return i;
	}
	ASSERTION(false); // Si le nom n'est pas trouvé, lancer exception.
}

/*
 * \brief Ajout un arc au graphe
 *
 * \param[in] source, l'index du sommet source.
 * \param[in] destination, l'index du sommet destination.
 * \param[in] duree, la valeur de la durée (float).
 * \param[in] cout, la valeur du coût (float).
 *
 * \pre La source et la destination passés en paramètre sont plus petit que le nombre de sommets.
 * \pre La durée et le coût sont non-négatifs.
 */
void Graphe::ajouterArc(size_t source, size_t destination, float duree,
		float cout) {
	ASSERTION(static_cast<int>(source) < getNombreSommets() and static_cast<int>(destination) < getNombreSommets());
	ASSERTION(duree >= 0.0 and cout >= 0.0);
	auto tmp = new Graphe::Arc(destination, Ponderations(duree, cout));
	listesAdj[source].push_back(*tmp);
	nbArcs++;
}

/*
 * \brief Vérifie si un arc entre deux sommets existe.
 *
 * \param[in] source, l'index du sommet source.
 * \param[in] destination, l'index du sommet destination.
 * \param[out] boolean, true si l'arc en question existe.
 *
 * \pre La source et la destination passées en paramètres sont plus petites que le nombre de sommets.
 */
bool Graphe::arcExiste(size_t source, size_t destination) const {
	ASSERTION(static_cast<int>(source) < getNombreSommets() and static_cast<int>(destination) < getNombreSommets());
	for (auto arc : listesAdj[source]) {
		if (arc.destination == destination) return true;
	}
	return false;
}

/*
 * \brief Enlève un arc entre deux sommets.
 *
 * \param[in] source, l'index du sommet source.
 * \param[in] destination, l'index du sommet destination.
 *
 * \pre La source et la destination passées en paramètre sont plus petites que le nombre de sommets.
 * \pre L'arc entre la source et la destination existe.
 */
void Graphe::enleverArc(size_t source, size_t destination) {
	ASSERTION(static_cast<int>(source) < getNombreSommets() and static_cast<int>(destination) < getNombreSommets());
	ASSERTION(arcExiste(source, destination));
	for (auto itr = listesAdj[source].begin(); itr != listesAdj[source].end() ; ++itr){
		if ((*itr).destination == destination) {
			listesAdj[source].erase(itr);
			nbArcs--;
			return;
		}
	}
}

/*
 * \brief Retourne l'inverse du graphe actuel
 *
 * \pre Le nombre de sommets du graphe est plus grand que 0.
 * \post Le nombre de sommets et le nombre d'arcs du graphe inversé est égal aux nombres de sommets et d'arcs du graphe actuel
 *
 * \param[out] *inverse, un Graphe représentant l'inverse du graphe actuel.
 */
Graphe Graphe::inverse() const {
	ASSERTION(this->getNombreSommets() > 0);

	if (this->getNombreArcs() == 0) return *this;

	auto nbSommets = this->getNombreSommets();
	auto inverse = new Graphe(nbSommets);
	for (int i = 0 ; i < this->getNombreSommets() ; i++){
		for (int j = 0 ; j < this->getNombreSommets() ; j++){
			if (this->arcExiste(i, j)) {
				inverse->ajouterArc(j, i, this->getPonderationsArc(i, j).duree, this->getPonderationsArc(i, j).cout);
			}
		}
	}
	ASSERTION(this->getNombreSommets() == inverse->getNombreSommets());
	ASSERTION(this->getNombreArcs() == inverse->getNombreArcs());
	return *inverse;
}


/*
 * \brief Retourne la liste des sommets adjacents d'un sommet donné.
 *
 * \param[in] sommet, l'index du sommet à vérifier.
 * \param[out] *liste, un vecteur contenant les sommets adjacents au sommet source.
 *
 * \pre Le sommet passé en paramètre est plus petit que le nombre de sommets.
 */
std::vector<size_t> Graphe::listerSommetsAdjacents(size_t sommet) const {
	ASSERTION(static_cast<int>(sommet) < getNombreSommets());

	auto liste = new std::vector<size_t>;
	for (auto itr = listesAdj[sommet].begin(); itr != listesAdj[sommet].end() ; ++itr){
		liste->push_back((*itr).destination);
	}
	return *liste;
}

/*
 * \brief Retourne la pondération (coût, durée) d'un arc.
 *
 * \pre La source et la destination passés en paramètre sont plus petit que le nombre de sommets.
 * \pre L'arc entre la source et la destination existe.
 *
 * \param[in] source, le sommet source de l'arc recherché
 * \param[in] destination, le sommet destination de l'arc recherché
 * \param[out] La pondération de l'arc entre source et destination
 */
Ponderations Graphe::getPonderationsArc(size_t source,
		size_t destination) const {
	ASSERTION(static_cast<int>(source) < getNombreSommets() and static_cast<int>(destination) < getNombreSommets());
	ASSERTION(arcExiste(source, destination));

	for (auto itr = listesAdj[source].begin(); itr != listesAdj[source].end() ; ++itr) {
		if ((*itr).destination == destination) return ((*itr).poids);
	}
	ASSERTION(false); //Gestion d'erreur, ne devrait jamais être exécuté.
}

/*
 * \brief Vérifie si tous les arcs du graphe ont des pondérations non-négatives.
 *
 * \pre Le nombre de sommets du graphe est plus grand que 0.
 *
 * \param[out] booléen, true si tous les arcs ont des pondérations non-négatives, false sinon.
 */
bool Graphe::arcsSontNonNegatifs() const {
	ASSERTION(this->getNombreSommets() > 0);

	for (auto liste : listesAdj){
		for (auto arc : liste) {
			if (arc.poids.cout < 0 or arc.poids.duree < 0) return false;
		}
	}
	return true;
}
} //Fin du namespace
