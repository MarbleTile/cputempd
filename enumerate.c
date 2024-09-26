
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sensors/sensors.h>

int main(void) {
    sensors_init(NULL);
    const sensors_chip_name *name;
    int n_name = 0;
    const sensors_feature *feat;
    int n_feat = 0;
    const sensors_subfeature *subfeat;
    int n_subfeat = 0;
    while ((name = sensors_get_detected_chips(NULL, &n_name)) != NULL) {
        printf("%s\t(%s)\n", name->prefix, name->path);
        while ((feat = sensors_get_features(name, &n_feat)) != NULL) {
            printf("|\n----%s\t(%s, %d)\n", feat->name, sensors_get_label(name, feat), feat->type);
            while ((subfeat = sensors_get_all_subfeatures(name, feat, &n_subfeat)) != NULL) {
                printf("\t|\n\t----%s\t(%d)\n", subfeat->name, subfeat->type);
            }
            n_subfeat = 0;
        }
        n_feat = 0;
        printf("\n");
    }
    sensors_cleanup();
    return 0;
}

