#ifndef MYVISITOR_H
#define MYVISITOR_H

#include <QStack>

#include <private/qqmljsast_p.h>          //QQmlJS::AST::Node
#include <private/qqmljsastvisitor_p.h>   //QQmlJS::AST::Visitor

class AstContext
{
public:
    QStringList m_nrProperties;
    QStringList m_nrFunctions;
    QStringList m_nrBindings;
    QStringList m_nrObjects;
};

class CheckingVisitor : public QQmlJS::AST::Visitor
{
public:
    CheckingVisitor(const QString &code);
    virtual ~CheckingVisitor();

    // QQmlJS::AST::Visitor implementation

    bool visit(QQmlJS::AST::UiPublicMember * a_arg) Q_DECL_OVERRIDE;
    void endVisit(QQmlJS::AST::UiPublicMember * a_arg) Q_DECL_OVERRIDE;

    bool visit(QQmlJS::AST::UiSourceElement * a_arg) Q_DECL_OVERRIDE;
    void endVisit(QQmlJS::AST::UiSourceElement * a_arg) Q_DECL_OVERRIDE;

    bool visit(QQmlJS::AST::UiScriptBinding * a_arg) Q_DECL_OVERRIDE;
    void endVisit(QQmlJS::AST::UiScriptBinding * a_arg) Q_DECL_OVERRIDE;

    bool visit(QQmlJS::AST::UiObjectBinding * a_arg) Q_DECL_OVERRIDE;
    void endVisit(QQmlJS::AST::UiObjectBinding * a_arg) Q_DECL_OVERRIDE;

    bool visit(QQmlJS::AST::UiObjectDefinition * a_arg) Q_DECL_OVERRIDE;
    void endVisit(QQmlJS::AST::UiObjectDefinition *) Q_DECL_OVERRIDE;

    // MyVisitor implementation
    bool hasWarnings() const { return !m_warnings.isEmpty(); }
    QStringList getWarnings() const { return m_warnings; }

private:
    QString getCode(const QQmlJS::AST::SourceLocation &a_arg) const;
    QString indent();
    QStringList m_warnings;

    QString m_code;

    QStack<AstContext> m_stack;
};

#endif // MYVISITOR_H
