#ifndef __REPSHEET_H
#define __REPSHEET_H

#define TRUE 1
#define FALSE 0

#define WHITELISTED 2
#define MARKED 3
#define BLACKLISTED 4
#define UNSUPPORTED 5
#define OK 6

#define IP 7
#define USER 8

#define DISCONNECTED -1

typedef struct repsheet_rule_t {
  char *part;
  char *location;
  char *field;
  char *error;
} repsheet_rule_t;

redisContext *get_redis_context(const char *host, int port, int timeout);
int check_connection(redisContext *context);

int increment_rule_count(redisContext *context, const char *actor, char *rule);

int mark_actor(redisContext *context, const char *actor, int type);
int blacklist_actor(redisContext *context, const char *actor, int type);
int whitelist_actor(redisContext *context, const char *actor, int type);

int is_ip_marked(redisContext *context, const char *actor);
int is_ip_blacklisted(redisContext *context, const char *actor);
int is_ip_whitelisted(redisContext *context, const char *actor);
int is_user_marked(redisContext *context, const char *actor);
int is_user_blacklisted(redisContext *context, const char *actor);
int is_user_whitelisted(redisContext *context, const char *actor);
int actor_status(redisContext *context, const char *actor, int type);

int is_historical_offender(redisContext *context, const char *actor);

int expire(redisContext *context, const char *actor, char *label, int expiry);
int blacklist_and_expire(redisContext *context, const char *actor, int expiry, char *reason);

int record(redisContext *context, char *timestamp, const char *user_agent,
            const char *http_method, char *uri, char *arguments, int redis_max_length,
            int redis_expiry, const char *actor);

const char *remote_address(char *connected_address, const char *xff_header);

int modsecurity_total(const char *waf_score);
void process_mod_security_headers(const char *waf_events, char *events[]);
int matches(const char *waf_events);

repsheet_rule_t process_rule(char *rule_string);

#endif