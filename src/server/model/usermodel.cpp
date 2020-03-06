#include "usermodel.hpp"
#include "db.h"
#include "dbpool.hpp"
#include <memory>
#include <iostream>
using namespace std;

// User表的增加方法
bool UserModel::insert(User &user)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into user(name, password, state) values('%s', '%s', '%s')",
            user.getName().c_str(), user.getPwd().c_str(), user.getState().c_str());
    shared_ptr<MySQL> mysql = ConnectionPool::getConnectionPool()->getConnection();
    if (mysql!=nullptr)
    {
        if (mysql->update(sql))
        {
            // 获取插入成功的用户数据生成的主键id
            user.setId(mysql_insert_id(mysql->getConnection()));
            return true;
        }
    }

    return false;
}

// 根据用户号码查询用户信息
User UserModel::query(int id)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select * from user where id = %d", id);

    shared_ptr<MySQL> mysql = ConnectionPool::getConnectionPool()->getConnection();
    if (mysql!=nullptr)
    {
        MYSQL_RES *res = mysql->query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row != nullptr)
            {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setPwd(row[2]);
                user.setState(row[3]);
                mysql_free_result(res);
                return user;
            }
        }
    }

    return User();
}

// 更新用户的状态信息
bool UserModel::updateState(User user)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "update user set state = '%s' where id = %d", user.getState().c_str(), user.getId());

    shared_ptr<MySQL> mysql = ConnectionPool::getConnectionPool()->getConnection();
    if (mysql!=nullptr)
    {
        if (mysql->update(sql))
        {
            return true;
        }
    }
    return false;
}

// 重置用户的状态信息
void UserModel::resetState()
{
    // 1.组装sql语句
    char sql[1024] = "update user set state = 'offline' where state = 'online'";

    shared_ptr<MySQL> mysql = ConnectionPool::getConnectionPool()->getConnection();
    if (mysql!=nullptr)
    {
        mysql->update(sql);
    }
}