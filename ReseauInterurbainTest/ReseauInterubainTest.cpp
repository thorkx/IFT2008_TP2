#include <gtest/gtest.h>
#include <ReseauInterurbain.h>

using namespace TP2;
using namespace std;

TEST (Graphe, ConstructeurNbSommetsDefautOK){
	auto tmp = new Graphe();
	ASSERT_EQ(tmp->getNombreSommets(), 10);
	ASSERT_NE(tmp->getNombreSommets(), 0);
}

TEST (Graphe, ConstructeurNbSommetsOK){
	auto tmp = new Graphe(2);
	ASSERT_EQ(tmp->getNombreSommets(), 2);
	ASSERT_NE(tmp->getNombreSommets(), 10);
}

TEST (Graphe, ConstructeurNbArcsDefautOK){
	auto tmp = new Graphe();
	ASSERT_EQ(tmp->getNombreArcs(), 0);
	ASSERT_NE(tmp->getNombreArcs(), 1);
}

TEST (Graphe, NommerSommetOK) {
	auto tmp = new Graphe();
	string test = "Sommet 0";
	tmp->nommer(0, test);
	ASSERT_EQ(tmp->getNomSommet(0), test);
}

TEST (Graphe, GrapheConstruitModifierNbSommetsOK){
	auto tmp = new Graphe(2);
	tmp->resize(4);
	ASSERT_EQ(tmp->getNombreSommets(), 4);
}

TEST (Graphe, GrapheConstruitModifierNbSommetsPasOK){
	auto tmp = new Graphe(2);
	ASSERT_ANY_THROW(tmp->resize(1));
}

TEST (Graphe, AjoutUnArcNbArcsOK) {
	auto tmp = new Graphe();
	tmp->ajouterArc(0, 1, 5, 5);
	ASSERT_EQ(tmp->getNombreArcs(), 1);
}


TEST (Graphe, EnleveUnArcNbArcsOK) {
	auto tmp = new Graphe();
	tmp->ajouterArc(0, 1, 5, 5);
	tmp->enleverArc(0,1);
	ASSERT_EQ(tmp->getNombreArcs(), 0);
}

TEST (Graphe, EnleveUnArcArcNExistePasOK) {
	auto tmp = new Graphe();
	tmp->ajouterArc(0, 1, 5, 5);
	tmp->enleverArc(0,1);
	ASSERT_TRUE(!tmp->arcExiste(0,1));
}

TEST (Graphe, AjoutUnArcArcExisteOK){
	auto tmp = new Graphe();
	tmp->ajouterArc(0, 1, 5, 5);
	ASSERT_TRUE(tmp->arcExiste(0,1));
}

TEST (Graphe, PasArcArcExisteRetourneFauxOK){
	auto tmp = new Graphe();
	ASSERT_TRUE(!tmp->arcExiste(0,1));
}

TEST (Graphe, Ponderation){
	auto tmp = new Graphe();
	Ponderations p = Ponderations(5, 5);
	tmp->ajouterArc(0, 1, 5, 5);
	ASSERT_TRUE((p.cout == tmp->getPonderationsArc(0,1).cout) and (p.duree == tmp->getPonderationsArc(0,1).duree));
}

TEST (Graphe, GrapheConstruitModifierNbSommetsRenommetNouveauSommetOK){
	auto tmp = new Graphe(2);
	auto test = "Test";
	tmp->resize(3);
	tmp->nommer(2, test);
	ASSERT_NO_THROW(tmp->ajouterArc(2, 1, 4, 4));
}

TEST (Graphe, GrapheConstruitModifierNbSommetsAjoutArcNouveauSommetOK){
	auto tmp = new Graphe(2);
	tmp->resize(3);
}

TEST (Graphe, SommetNommeEstTrouveOK) {
	auto tmp = new Graphe(2);
	tmp->nommer(0, "Test");
	ASSERT_EQ(tmp->getNumeroSommet("Test"), 0);
}

TEST (Graphe, SommetNommePasTrouveLanceException) {
	auto tmp = new Graphe(2);
	tmp->nommer(0, "Test");
	ASSERT_ANY_THROW(tmp->getNumeroSommet("Mauvais test"));
}

TEST (Reseau, ParcoursEnProfondeur) {
	auto r = new ReseauInterurbain("test", 6);
	r->test();
}
