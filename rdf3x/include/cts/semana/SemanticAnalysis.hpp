#ifndef H_cts_semana_SemanticAnalysis
#define H_cts_semana_SemanticAnalysis
//---------------------------------------------------------------------------
#include <dblayer.hpp>
#include <string>
//---------------------------------------------------------------------------
// RDF-3X
// (c) 2008 Thomas Neumann. Web site: http://www.mpi-inf.mpg.de/~neumann/rdf3x
//
// This work is licensed under the Creative Commons
// Attribution-Noncommercial-Share Alike 3.0 Unported License. To view a copy
// of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/
// or send a letter to Creative Commons, 171 Second Street, Suite 300,
// San Francisco, California, 94105, USA.
//---------------------------------------------------------------------------
//class Database;
//class DictionarySegment;
class DifferentialIndex {
}; //DifferentialIndex is not used. I rename it as an empty class

class SPARQLParser;
class QueryGraph;
class QueryDict;
//---------------------------------------------------------------------------
/// Semantic anaylsis for SPARQL queries. Transforms the parse result into a query graph
class SemanticAnalysis {
public:
    /// A semantic exception
    struct SemanticException {
        /// The message
        std::string message;

        /// Constructor
        SemanticException(const std::string& message);
        /// Constructor
        SemanticException(const char* message);
        /// Destructor
        ~SemanticException();
    };

private:
    /// The dictionary. Used for string and IRI resolution
    //DictionarySegment& dict;
    DBLayer& dict;

    /// The differential index (if any)
    //DifferentialIndex* diffIndex;
    /// A temporary dictionary
    QueryDict &tempDict;

public:
    /// Constructor
    explicit SemanticAnalysis(DBLayer& db, QueryDict &tempDict);
    /// Constructor
    //explicit SemanticAnalysis(DifferentialIndex& diffIndex, QueryDict &tempDict);

    /// Perform the transformation
    void transform(const SPARQLParser& input, QueryGraph& output);
};
//---------------------------------------------------------------------------
#endif
