#ifndef QUERYMANAGER_H
#define QUERYMANAGER_H
#include "QString"
#include "QToolButton"

class QueryManager
{
public:
    /**
     * @brief QueryManager
     * Constructor for class QueryManager
     */
    QueryManager();

    /**
     * @brief isBeginning
     * @return True if the current query is the first query in the query line, false otherwise
     */
    bool isBeginning() const;

    /**
     * @brief isFinal
     * @return True if the current query is the last query in the query line, false otherwise
     */
    bool isFinal() const;

    /**
     * @brief getPreviousQuery
     * @return The completed previous query
     */
    const QString& getPreviousQuery();

    /**
     * @brief getSubsequentQuery
     * @return The completed subsequent query
     */
    const QString& getSubsequentQuery();

    /**
     * @brief storeQuery
     * Stores the provided query, including all subitems.
     * @param query The completed query
     * @param queries Vector of all non intervention query items
     * @param conditions Vector of all condition query items
     * @param interventions Vector of all intervention query items
     * @param edgeChanges Vector of all edge Change operation items
     */
    void storeQuery(const QString &query, const std::vector<QString>& queries,
                    const std::vector<QString>& conditions, const std::vector<QString>& interventions,
                    const std::vector<QString>& edgeChanges);

    /**
     * Return the index of the currently selected query.
     */
    unsigned int getCurrentQuery() const;

    /**
     * @brief getNumberOfQueries
     * @return The number of stored queries
     */
    unsigned int getNumberOfQueries() const;

    /**
     * Get the collection of queries.
     */
    const std::vector<QString>& getQueries() const;

    /**
     * @brief getQuery
     * @param index The index of the desired query
     * @return The query at the given index
     */
    const QString& getQuery(unsigned int index) const;

    /**
     * @brief getQueryItems
     * @param index The index of the desired query
     * @return A vector of the non intervention query items of the query with the given index
     */
    const std::vector<QString>& getQueryItems(unsigned int index) const;

    /**
     * @brief getConditionItems
     * @param index The index of the desired query
     * @return A vector of the conditional query items of the query with the given index
     */
    const std::vector<QString>& getConditionItems(unsigned int index) const;

    /**
     * @brief getInterventionItems
     * @param index The index of the desired query
     * @return A vector of the intervention items of the query with the given index
     */
    const std::vector<QString>& getInterventionItems(unsigned int index) const;

    /**
     * @brief getEdgeAddRemItems
     * @param index The index of the desired query
     * @return A vector of the edge change items of the query with the given index
     */
    const std::vector<QString>& getEdgeAddRemItems(unsigned int index) const;

    /**
     * @brief storeQuery
     * Stores the provied line in the query vector
     * @param line Query to store
     */
    void storeQuery(const QString& line);

    /**
     * @brief storeQueryItem
     * Stores the provided query item
     * @param line query item to store
     */
    void storeQueryItem(QString line);

    /**
     * @brief storeConditionItem
     * Stores the provided condition item
     * @param line condition item to store
     */
    void storeConditionItem(QString line);

    /**
     * @brief storeInterventionItem
     * Stores the provided intervention item
     * @param line query item to store
     */
    void storeInterventionItem(QString line);

    /**
     * @brief storeAddRemEdgeItem
     * Stores the provided edge change item
     * @param line query item to store
     */
    void storeAddRemEdgeItem(QString line);


private:
    /**
     * @brief queryVector_
     * Vector containing the complet queries
     */
    std::vector<QString> queryVector_;

    /**
     * @brief queryItems_
     * Vector containing the query items for each complet query
     */
    std::vector<std::vector<QString>> queryItems_;

    /**
     * @brief conditionItems_
     * Vector containing the condition items for each complet query
     */
    std::vector<std::vector<QString>> conditionItems_;

    /**
     * @brief interventionItems_
     * Vector containing the intervention items for each complet query
     */
    std::vector<std::vector<QString>> interventionItems_;

    /**
     * @brief edgeAddRemItems_
     * Vector containing the edge change items for each complet query
     */
    std::vector<std::vector<QString>> edgeAddRemItems_;

    /**
     * @brief index_
     * Index of the current query
     */
    unsigned int index_;
};

#endif // QUERYMANAGER_H
