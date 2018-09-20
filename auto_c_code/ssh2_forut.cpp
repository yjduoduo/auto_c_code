#include "ssh2_forut.h"
#include "libssh/ssh2.h"
#include "libssh/libssh.h"
#include "libssh/server.h"
#include "libssh/sftp.h"
#include <QDebug>
#include <errno.h>
#include <string.h>
#include "QSshSocket/qsshsocket.h"

/**
 ** ��������
 **/
int verify_knownhost(ssh_session session);

//ssh2_forut::ssh2_forut()
//{
//    ssh_session my_ssh_session = ssh_new();
//    if (my_ssh_session == NULL)
//        exit(-1);
//    ssh_options_set(my_ssh_session,SSH_OPTIONS_HOST,"192.168.1.10");
//    ssh_options_set(my_ssh_session,SSH_OPTIONS_PORT,"21");

//    ssh_free(my_ssh_session);
//}

ssh2_forut::ssh2_forut(char *ipaddr, QTextEdit *showresult)
{
    m_showresult = showresult;
    //192.168.59.132
    snprintf(m_ipaddr, sizeof(m_ipaddr), "%s",ipaddr);
    qDebug() << "one ssh2 test, coming..., ipaddr:" << ipaddr << ", m_ipaddr:" << m_ipaddr;
}

void ssh2_forut::settext(QString text)
{
    m_showresult->append(text);
}

//SSH��������
//http://api.libssh.org/master/libssh_tutor_guided_tour.html
void ssh2_forut::whenconnected()
{
    qDebug() << "one ssh2 test, connecting...";
    ssh_session my_ssh_session;
    int rc;
    char *password;
    int verbosity = SSH_LOG_PROTOCOL;
    int port = 22;
    my_ssh_session = ssh_new();
    if (my_ssh_session == NULL)
        return;
    //    ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, "localhost");
    ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, m_ipaddr);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);

    verify_knownhost(my_ssh_session);

    rc = ssh_connect(my_ssh_session);
    if (rc != SSH_OK)
    {
        qDebug("Error connecting to ipaddr: %s\n",
               ssh_get_error(my_ssh_session));
        settext(QString("Error connecting to ipaddr: %1\n").arg(ssh_get_error(my_ssh_session))
                );
        return;
    }
    qDebug("connecting ok!! %s\n",
           ssh_get_error(my_ssh_session));
    settext(QString("connecting ok!! %1\n").arg(ssh_get_error(my_ssh_session))
            );
    // Verify the server's identity
    // For the source code of verify_knowhost(), check previous example
    if (verify_knownhost(my_ssh_session) < 0)
    {
        settext("verify error!\n");
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        return;
    }
    settext("verify ok!\n");

    // Authenticate ourselves
//    password = getpass("Password: ");
    password = "1230...";
    rc = ssh_userauth_password(my_ssh_session, NULL, password);
    if (rc != SSH_AUTH_SUCCESS)
    {
        settext(QString("Error authenticating with password: %1\n").arg(ssh_get_error(my_ssh_session))
                );
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        exit(-1);
    }

    ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);
    settext("close!\n");
}



int verify_knownhost(ssh_session session)
{
    int state, hlen;
    unsigned char *hash = NULL;
    char *hexa;
    char buf[10];
    state = ssh_is_server_known(session);
    hlen = ssh_get_pubkey_hash(session, &hash);
    if (hlen < 0)
    {
        qDebug( "hlen inalid!!\n");
        return -1;
    }
    switch (state)
    {
    case SSH_SERVER_KNOWN_OK:
        break; /* ok */
    case SSH_SERVER_KNOWN_CHANGED:
        qDebug( "Host key for server changed: it is now:\n");
        ssh_print_hexa("Public key hash", hash, hlen);
        qDebug( "For security reasons, connection will be stopped\n");
        free(hash);
        return -1;
    case SSH_SERVER_FOUND_OTHER:
        qDebug( "The host key for this server was not found but an other"
                "type of key exists.\n");
        qDebug( "An attacker might change the default server key to"
                "confuse your client into thinking the key does not exist\n");
        free(hash);
        return -1;
    case SSH_SERVER_FILE_NOT_FOUND:
        qDebug( "Could not find known host file.\n");
        qDebug( "If you accept the host key here, the file will be"
                "automatically created.\n");
        /* fallback to SSH_SERVER_NOT_KNOWN behavior */
    case SSH_SERVER_NOT_KNOWN:
        hexa = ssh_get_hexa(hash, hlen);
        qDebug("The server is unknown. Do you trust the host key?\n");
        qDebug( "Public key hash: %s\n", hexa);
        free(hexa);
        //        if (fgets(buf, sizeof(buf), stdin) == NULL)
        //        {
        //            free(hash);
        //            return -1;
        //        }

        //        if (strncasecmp(buf, "yes", 3) != 0)
        //        {
        //            free(hash);
        //            return -1;
        //        }
        if (ssh_write_knownhost(session) < 0)
        {
            qDebug( "Error %s\n", strerror(errno));
            free(hash);
            return -1;
        }
        break;
    case SSH_SERVER_ERROR:
        qDebug( "Error %s", ssh_get_error(session));
        free(hash);
        return -1;
    }
    free(hash);
    return 0;
}


