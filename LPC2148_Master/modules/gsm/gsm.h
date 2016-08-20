#ifndef GSM_H
#define	GSM_H

void gsm_cmd(char *cmd,char *Response,unsigned int timeout);
void gsm_init(void);
void gsm_pname(char *pname);
void gsm_msg_send(char *num,char *msg);
unsigned int gsm_wait(char *mid);
void gsm_msg_read(char *num,char *msg,char *idn);

void debug(char *str);

#endif	/* GSM_H */
