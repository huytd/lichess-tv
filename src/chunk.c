/* Chunk Parser
 *
 * This is a set of parser functions that help dealing with the
 * chunk data streamed from Lichess API.
 *
 * Everytime we get a chunk, we will need to call the chunk_parse
 * method to start the parse. The parsed result will be stored in
 * the current_root struct.
 *
 * Every subsequence calls will use the current_root as the data
 * source.
 *
 * When everything is done, we will need to call chunk_destroy to
 * free up the parsed data.
 */

#include "chunk.h"

static struct json_value_s* current_root;

struct json_object_element_s*
find_element_by_name(char* name)
{
    struct json_object_s* obj = (struct json_object_s*)current_root->payload;
    struct json_object_element_s* cur =
      (struct json_object_element_s*)obj->start;
    while (strcmp(cur->name->string, name) != 0) {
        if (cur->next != NULL) {
            cur = cur->next;
        } else {
            return NULL;
        }
    }
    return cur;
}

struct json_object_element_s*
find_element_by_name_from(struct json_object_element_s* element, char* name)
{
    struct json_object_s* obj = (struct json_object_s*)element;
    struct json_object_element_s* cur =
      (struct json_object_element_s*)obj->start;
    while (cur != NULL && strcmp(cur->name->string, name) != 0) {
        if (cur->next != NULL) {
            cur = cur->next;
        } else {
            return NULL;
        }
    }
    return cur;
}

void
chunk_parse(char* chunk, size_t len)
{
    current_root = json_parse(chunk, len);
}

int
chunk_is_move_description()
{
    if (current_root != NULL) {
        struct json_object_element_s* tag = find_element_by_name("t");
        if (tag != NULL) {
            struct json_string_s* value =
              (struct json_string_s*)tag->value->payload;
            return strcmp(value->string, "fen") == 0;
        }
    }
    return 0;
}

const char*
chunk_get_fen()
{
    if (current_root != NULL) {
        struct json_object_element_s* data = find_element_by_name("d");
        if (data != NULL) {
            struct json_object_element_s* data_el =
              (struct json_object_element_s*)data->value->payload;
            struct json_object_element_s* fen =
              find_element_by_name_from(data_el, "fen");
            if (fen != NULL) {
                struct json_string_s* fen_value =
                  (struct json_string_s*)fen->value->payload;
                return fen_value->string;
            }
        }
    }
    return NULL;
}

void
parse_player(player_t* player, struct json_object_element_s* obj)
{
    struct json_object_element_s* user =
      (struct json_object_element_s*)find_element_by_name_from(obj, "user")
        ->value->payload;

    const char* rating_src =
      json_value_as_number(find_element_by_name_from(obj, "rating")->value)
        ->number;
    const char* name_src =
      json_value_as_string(find_element_by_name_from(user, "name")->value)
        ->string;

    // These JSON strings will be free when chunk_destroy is called
    // so, we need to copy these name strings into somewhere safe
    player->rating = (char*)malloc((strlen(rating_src) + 1) * sizeof(char));
    player->name   = (char*)malloc((strlen(name_src) + 1) * sizeof(char));
    strcpy(player->rating, rating_src);
    strcpy(player->name, name_src);
}

void
chunk_parse_players(player_t* ref_players)
{
    if (current_root != NULL) {
        struct json_object_element_s* data = find_element_by_name("d");
        if (data != NULL) {
            struct json_object_element_s* data_el =
              (struct json_object_element_s*)data->value->payload;
            struct json_object_element_s* players =
              find_element_by_name_from(data_el, "players");
            if (players != NULL) {
                struct json_array_s* players_arr =
                  json_value_as_array(players->value);

                struct json_array_element_s* player_w = players_arr->start;
                struct json_object_element_s* player_w_obj =
                  (struct json_object_element_s*)player_w->value->payload;
                ref_players[0].is_black = 0;
                parse_player(&ref_players[0], player_w_obj);

                struct json_array_element_s* player_b = player_w->next;
                struct json_object_element_s* player_b_obj =
                  (struct json_object_element_s*)player_b->value->payload;
                ref_players[1].is_black = 1;
                parse_player(&ref_players[1], player_b_obj);
            }
        }
    }
}

void
chunk_destroy()
{
    if (current_root != NULL) {
        free(current_root);
        current_root = NULL;
    }
}
