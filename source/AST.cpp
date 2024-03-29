#include "AST.h"
#include "SimpleParser.h"

AST::AST(){
    TNode* rootNode = new TNode(ProcedureName, "");
    rootNode->addChild(new TNode(StmtLst, ""));
    tree.push_back(rootNode);
}

AST::AST(std::string name){
    TNode* rootNode = new TNode(ProcedureName, name);
    rootNode->addChild(new TNode(StmtLst, ""));
    tree.push_back(rootNode);
}

AST::~AST(){
}

std::vector<TNode*> AST::getTree() {
	return tree;
}

TNode* AST::findNode(TNode* node) {
	for (unsigned i = 0; i < tree.size(); i++) {
		if (node == tree[i]) {
			return tree[i];
		}
	}
	return new TNode();
}

int AST::getNodeIndex(TNode* node) {
    for (unsigned int i = 0; i < tree.size(); i++) {
        if (tree[i] == node) {
            return i;
        }
    }
    return -1;
}

void AST::addToTree(TNode* node) {
	tree.push_back(node);
}

void AST::makeChild(TNode* parent, TNode* child) {
    parent->getChildNodes().back()->addChild(child);
}

void AST::makeParent(TNode* child, TNode* parent) {
    parent->getChildNodes().back()->addChild(child);
}

TNode* AST::appendNewStmtNode(TNodeType type, std::string value, int stmtNum) {
    TNode* newNode = new TNode(type,value,stmtNum);

    if (tree.back()->isContainerStmt()) {
        AST::makeChild(tree.back(), newNode);
    } else {
		if (endOfContainerStmt) {
			TNode* currentNode = tree.back();
			for (int i = numberOfEndContainerCalls; i != 0; i--) {
				currentNode = getParent(currentNode);
			}
			numberOfEndContainerCalls = 0;
			AST::makeChild(AST::getParent(currentNode), newNode);
			endOfContainerStmt = false;
		} else {
			AST::makeChild(AST::getParent(tree.back()), newNode);
		}
    }

    tree.push_back(newNode);
    return newNode;
}

TNode* AST::getLastAddedNode() {
    return tree.back();
}

std::vector<TNode*> AST::getChildren(TNode* node) {
    std::vector<TNode*> result;
    
    if (node->getChildNodes().size() == 2) {
        return node->getChildNodes()[1]->getChildNodes();
    } else if (node->getChildNodes().size() == 3) {
        result = node->getChildNodes()[1]->getChildNodes();
        result.insert(result.end(), node->getChildNodes()[2]->getChildNodes().begin(), 
            node->getChildNodes()[2]->getChildNodes().end());
    }

    return result;
}

TNode* AST::getParent(TNode* node) {
	return node->getParent()->getParent();
}

TNode* AST::processAssignmentStmt(NAME variable, std::vector<std::string> &tokens) {
    std::vector<std::string> RHS;
    if (tokens.size() > 2 && tokens[1] == "=") {
        RHS = std::vector<std::string>(tokens.begin() + 2, tokens.end());
    } else {
        RHS = std::vector<std::string>(tokens.begin(), tokens.end());
    }
    TNode* equals = new TNode(OperatorEquals, "");
    TNode* leftSub = new TNode(VariableName, variable);
    TNode* rightSub = AST::constructExpressionTree(RHS);

    equals->addChild(leftSub);
    equals->addChild(rightSub);

    return equals;
}

TNode* AST::constructExpressionTree(std::string expression) {
    std::vector<std::string> tokens = SimpleParser::tokenize(expression);
    return AST::constructExpressionTree(tokens);
}


TNode* AST::constructExpressionTree(std::vector<std::string> &tokens){
    std::stack<TNode*> operatorstk;
    std::stack<TNode*> operandstk;

    if (tokens.size() == 0) {
        return NULL;
    }

    for (int i = ((int)tokens.size() - 1); i > -1; i--) {
        //ignore underscores
        if (tokens[i] == "_") {
            continue;
        }

        TNode* node = new TNode();
        
        //If token is + or -, check if there are any * operators on the top of the 
        //stack and resolve them
        if (tokens[i] == "+") {
            node->setNodeType(OperatorPlus);
            while (!operatorstk.empty() && operatorstk.top()->getNodeType() == OperatorTimes){
                TNode* opr = operatorstk.top();
                operatorstk.pop();
                TNode* op2 = operandstk.top();
                operandstk.pop();
                TNode* op1 = operandstk.top();
                operandstk.pop();
                opr->addChild(op2);
                opr->addChild(op1);
                operandstk.push(opr);
            }
            operatorstk.push(node);
        } else if (tokens[i] == "-") {
            node->setNodeType(OperatorMinus);
            while (!operatorstk.empty() && operatorstk.top()->getNodeType() == OperatorTimes) {
                TNode* opr = operatorstk.top();
                operatorstk.pop();
                TNode* op2 = operandstk.top();
                operandstk.pop();
                TNode* op1 = operandstk.top();
                operandstk.pop();
                opr->addChild(op2);
                opr->addChild(op1);
                operandstk.push(opr);
            }
            operatorstk.push(node);
        //otherwise if operator, just push onto stack
        } else if (tokens[i] == "*") {
            node->setNodeType(OperatorTimes);
            operatorstk.push(node);
        }
        else if (tokens[i] == ")") {
            node->setNodeType(RightParenthesis);
            operatorstk.push(node);
        }
        //except for ). Resolve all operators until you meet a LeftParenthesis
        else if (tokens[i] == "(") {
            node->setNodeType(LeftParenthesis);
            while (!operatorstk.empty() && operatorstk.top()->getNodeType() != RightParenthesis) {
                TNode* opr = operatorstk.top();
                operatorstk.pop();
                TNode* op2 = operandstk.top();
                operandstk.pop();
                TNode* op1 = operandstk.top();
                operandstk.pop();
                opr->addChild(op2);
                opr->addChild(op1);
                operandstk.push(opr);
            }
            //get rid of the left parenthesis
            operatorstk.pop();
        }
        //otherwise push onto operand stack
        else {
            node->setNodeType(VariableName);
            node->setValue(tokens[i]);
            operandstk.push(node);
        }
    }
    
    //resolve all remaining operators on the stack
    while (!operatorstk.empty()) {
        TNode* opr = operatorstk.top();
        operatorstk.pop();
        TNode* op2 = operandstk.top();
        operandstk.pop();
        TNode* op1 = operandstk.top();
        operandstk.pop();
        opr->addChild(op2);
        opr->addChild(op1);
        operandstk.push(opr);
    }

    //returns root of expression tree
    return operandstk.top();
}

void AST::convertExpressionToTNodes(std::string expression, std::vector<TNode*> &resVector) {
    std::vector<std::string> tokens = SimpleParser::tokenize(expression);
    for (size_t i = 0; i < tokens.size(); i++) {
        TNode* node = new TNode();
        if (tokens[i] == "+") {
            node->setNodeType(OperatorPlus);
        }
        else if (tokens[i] == "-") {
            node->setNodeType(OperatorMinus);
        }
        else if (tokens[i] == "*") {
            node->setNodeType(OperatorTimes);
        }
        else if (tokens[i] == "(") {
            node->setNodeType(LeftParenthesis);
        }
        else if (tokens[i] == ")") {
            node->setNodeType(RightParenthesis);
        }
        else {
            node->setNodeType(VariableName);
            node->setValue(tokens[i]);
        }

        resVector.push_back(node);
    }
}

bool AST::compareTrees(TNode * tree1, TNode * tree2){
    if (tree1->getValue() != tree2->getValue()) {
        return false;
    } else if (tree1->getNodeType() != tree2->getNodeType()) {
        return false;
    } else if ((tree1->getChildNodes()).size() != (tree2->getChildNodes()).size()) {
        return false;
    } else {
        for (size_t i = 0; i < tree1->getChildNodes().size(); i++){
            if (!AST::compareTrees(tree1->getChildNodes()[i], tree2->getChildNodes()[i])) {
                return false;
            }
        }
    }
    return true;
}

bool AST::findSubtreeInAST(TNodeType type, TNode * subtree){
    for (unsigned i = 0; i < tree.size(); i++) {
        if (type == tree[i]->getNodeType()) {
            if (AST::findSubtreeInTree(subtree, tree[i])) {
                return true;
            }
        }
    }
    return false;
}

bool AST::findSubtreeInTree(TNode * subtree, TNode * tree){
    if (compareTrees(subtree, tree)) {
        return true;
    } else {
        for (unsigned int i = 0; i < tree->getChildNodes().size(); i++) {
            if (findSubtreeInTree(subtree, tree->getChildNodes()[i])) {
                return true;
            }
        }
    }

    return false;
}

TNode* AST::addAssignTNode(NAME variable, std::vector<std::string> tokens, int statementNumber) {
	TNode* newNode = this->appendNewStmtNode(Assign, "", statementNumber);
	this->getLastAddedNode()->addChild(this->processAssignmentStmt(variable, tokens));
    return newNode;
}

TNode* AST::addCallTNode(std::string procName, int statementNumber) {
    TNode* newNode = this->appendNewStmtNode(Call, procName, statementNumber);
    return newNode;
}

TNode* AST::addWhileTNode(NAME variable, int statementNumber) {
    TNode* newNode = this->appendNewStmtNode(While, "", statementNumber);
	this->getLastAddedNode()->addChild(new TNode(VariableName, variable));
	this->getLastAddedNode()->addChild(new TNode(StmtLst));
    return newNode;
}

TNode* AST::addIfTNode(NAME variable, int statementNumber) {
	TNode* newNode = this->appendNewStmtNode(If, "", statementNumber);
	this->getLastAddedNode()->addChild(new TNode(VariableName, variable));
	this->getLastAddedNode()->addChild(new TNode(StmtLst));
    return newNode;
}

void AST::addElseRelation() {
	TNode* lastNode = this->getLastAddedNode();
	while (lastNode->getNodeType() != If) {
		lastNode = lastNode->getParent();
	}
	lastNode->addChild(new TNode(StmtLst));
}

void AST::addEndOfContainerRelation() {
	numberOfEndContainerCalls += 1;
	endOfContainerStmt = true;
}