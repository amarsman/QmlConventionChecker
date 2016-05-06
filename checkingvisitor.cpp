#include "checkingvisitor.h"

CheckingVisitor::CheckingVisitor(const QString &code)
    : QQmlJS::AST::Visitor()
    , m_code(code)
{
    // Start with root context
    m_stack.push(AstContext());
}

CheckingVisitor::~CheckingVisitor()
{
    // Remove root context
    m_stack.pop();
}



/***************************************************************************************************
 * properties e.g. "property int a"
***************************************************************************************************/
bool CheckingVisitor::visit(QQmlJS::AST::UiPublicMember * a_arg)
{
    const QString token(getCode(a_arg->identifierToken));
    //qDebug() << indent() << "MEMBER" << token;

    if(!m_stack.top().m_functions.isEmpty())
    {
        QStringList a = QStringList() << "Functions ("
                                      << m_stack.top().m_functions
                                      << ") before property ("
                                      << token
                                      << ")";
        m_warnings.append(a.join(" "));
    }

    m_stack.top().m_properties.append(token);
    m_stack.push(AstContext());

    return true;
}

void CheckingVisitor::endVisit(QQmlJS::AST::UiPublicMember *)
{
    m_stack.pop();
}



/***************************************************************************************************
 * functions
***************************************************************************************************/
bool CheckingVisitor::visit(QQmlJS::AST::UiSourceElement * a_arg)
{
    QQmlJS::AST::FunctionDeclaration *funDecl =
            dynamic_cast<QQmlJS::AST::FunctionDeclaration *>(a_arg->sourceElement);
    Q_ASSERT(funDecl);

    const QString token(getCode(funDecl->identifierToken));

    if(!m_stack.top().m_bindings.isEmpty())
    {
        QStringList a = QStringList() << "Bindings ("
                                      << m_stack.top().m_bindings
                                      << ") before function ("
                                      << token
                                      << ")";
        m_warnings.append(a.join(" "));
    }

    m_stack.top().m_functions.append(token);
    m_stack.push(AstContext());

    return true;
}

void CheckingVisitor::endVisit(QQmlJS::AST::UiSourceElement *)
{
    m_stack.pop();
}



/***************************************************************************************************
 * assignments / bindings e.g. "height: 200"
***************************************************************************************************/
bool CheckingVisitor::visit(QQmlJS::AST::UiScriptBinding * a_arg)
{
    const QString token(getCode(a_arg->qualifiedId->identifierToken));

    if(!m_stack.top().m_objects.isEmpty())
    {
        QStringList a = QStringList() << "Objects ("
                                      << m_stack.top().m_objects
                                      << ") before binding ("
                                      << token
                                      << ")";
        m_warnings.append(a.join(" "));
    }

    if (token != "id" && token != "objectName")
    {
        m_stack.top().m_bindings.append(token);
    }

    m_stack.push(AstContext());

    return true;
}

void CheckingVisitor::endVisit(QQmlJS::AST::UiScriptBinding *)
{
    m_stack.pop();
}



/***************************************************************************************************
 * object assignments / bindings
***************************************************************************************************/
bool CheckingVisitor::visit(QQmlJS::AST::UiObjectBinding * a_arg)
{
    const QString token(getCode(a_arg->qualifiedId->identifierToken));

    if(!m_stack.top().m_objects.isEmpty())
    {
        QStringList a = QStringList() << "Objects ("
                                      << m_stack.top().m_objects
                                      << ") before binding ("
                                      << token
                                      << ")";
        m_warnings.append(a.join(" "));
    }

    m_stack.top().m_bindings.append(token);
    m_stack.push(AstContext());

    return true;
}

void CheckingVisitor::endVisit(QQmlJS::AST::UiObjectBinding *)
{
    m_stack.pop();
}



/***************************************************************************************************
 * objects e.g. "Item { }"
***************************************************************************************************/
bool CheckingVisitor::visit(QQmlJS::AST::UiObjectDefinition * a_arg)
{
    const QString token(getCode(a_arg->qualifiedTypeNameId->identifierToken));

    if (token != "anchors")
    {
        m_stack.top().m_objects.append(token);
    }

    m_stack.push(AstContext());

    return true;
}

void CheckingVisitor::endVisit(QQmlJS::AST::UiObjectDefinition *)
{
    m_stack.pop();
}



/**************************************************************************************************/
QString CheckingVisitor::getCode(const QQmlJS::AST::SourceLocation &a_arg) const
{
    return m_code.mid(a_arg.offset,a_arg.length);
}
