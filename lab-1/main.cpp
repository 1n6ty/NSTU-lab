#include "pirson/alt_pirson.h"
#include "mixd/mixd.h"
#include "empirical/empirical.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <limits>

double inputDouble(){
    double value;
    while(true){
        if(std::cin >> value){
            break;
        } else{
            std::cout << "Error, please input double!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return value;
}

int inputInt(){
    int value;
    while(true){
        if(std::cin >> value && value > 0){
            break;
        } else{
            std::cout << "Error, please input possitive integer!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return value;
}

int inputIntBorders(int min, int max){
    int value;
    while(true){
        if(std::cin >> value && value >= min && value <= max){
            break;
        } else{
            printf("Error, please input possitive integer between %d and %d!\n", min, max);
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return value;
}

int main(){
    Alt_Pirson_Status ps;
    Alt_Pirson_p *buf_p = new_Alt_Pirson_p(new_Pirson_p(1), 1, 1);

    Mix_Status ms;
    double coefs[2] = {0.5, 0.5}, *buf_darr;
    Alt_Pirson_p params[] = {*new_Alt_Pirson_p(new_Pirson_p(1), 1, 1), *new_Alt_Pirson_p(new_Pirson_p(1), 1, 1)};
    Mix_p *buf_mp = new_Mix_p(params, coefs, 2);
    
    Empirical_Status es;
    Empirical_p *buf_ep;

    int buf;
    double buf_d, buf_d1, buf_d2;
    
    while(true){
        std::cout << "Choose mode (0 - exit, 1 - Pirson-VII, 2 - Mix, 3 - Empirical): ";
        buf = inputIntBorders(0, 3);
        if(buf == 0){
            break;
        } else if(buf == 1){
            while(true){
                std::cout << "Pirson-VII: Choose mode (0 - exit, 1 - parameters only, 2 - generate x, 3 - generate density): ";
                buf = inputIntBorders(0, 3);
                if(buf == 0){
                    break;
                } else if(buf == 1){
                    std::cout << "Pirson-VII: Input shape coefficient: ";
                    buf_p->dp->v = inputDouble();
                    std::cout << "Pirson-VII: Input shear coefficient: ";
                    buf_p->u = inputDouble();
                    std::cout << "Pirson-VII: Input scale coefficient: ";
                    buf_p->a = inputDouble();

                    buf_d = pirson_compute_mat_expectation(buf_p, &ps);
                    std::cout << "Pirson-VII: Mat-Expectation: " << ((ps == ALT_PIRSON_SUCCESS) ? std::to_string(buf_d): "Doesn\'t exist") << '\n';
                    buf_d = pirson_compute_dispersion(buf_p, &ps);
                    std::cout << "Pirson-VII: Dispersion: " << ((ps == ALT_PIRSON_SUCCESS) ? std::to_string(buf_d): "Doesn\'t exist") << '\n';
                    buf_d = pirson_compute_skewness(buf_p, &ps);
                    std::cout << "Pirson-VII: Skewness: " << ((ps == ALT_PIRSON_SUCCESS) ? std::to_string(buf_d): "Doesn\'t exist\n") << '\n';
                    buf_d = pirson_compute_excess(buf_p, &ps);
                    std::cout << "Pirson-VII: Excess: " << ((ps == ALT_PIRSON_SUCCESS) ? std::to_string(buf_d): "Doesn\'t exist\n") << '\n';

                } else if(buf == 2){
                    std::cout << "Pirson-VII: Input shape coefficient: ";
                    buf_p->dp->v = inputDouble();
                    std::cout << "Pirson-VII: Input shear coefficient: ";
                    buf_p->u = inputDouble();
                    std::cout << "Pirson-VII: Input scale coefficient: ";
                    buf_p->a = inputDouble();
                    std::cout << "Pirson-VII: Input x count: ";
                    buf = inputInt();

                    std::ofstream file("output.txt", std::ios::trunc);
                    for(int i = 0; i < buf; i++){
                        buf_d = pirson_generate_x(buf_p, &ps);
                        if(ps != ALT_PIRSON_SUCCESS) break;

                        file << std::to_string(buf_d) << ' ';
                    }
                    file.close();

                    std::cout << "File \'output.txt\' was written!\n";
                } else if(buf == 3){
                    std::cout << "Pirson-VII: Input shape coefficient: ";
                    buf_p->dp->v = inputDouble();
                    std::cout << "Pirson-VII: Input shear coefficient: ";
                    buf_p->u = inputDouble();
                    std::cout << "Pirson-VII: Input scale coefficient: ";
                    buf_p->a = inputDouble();
                    std::cout << "Pirson-VII: Input f count: ";
                    buf = inputInt();
                    std::cout << "Pirson-VII: Input start x: ";
                    buf_d = inputDouble();
                    std::cout << "Pirson-VII: Input end x: ";
                    buf_d1 = inputDouble();

                    std::ofstream file("output.txt", std::ios::trunc);
                    for(double i = buf_d; i < buf_d1; i+=(buf_d1 - buf_d)/buf){
                        buf_d2 = pirson_compute_density(i, buf_p, &ps);
                        if(ps != ALT_PIRSON_SUCCESS) break;

                        file << std::to_string(buf_d2) << ' ';
                    }
                    file.close();

                    std::cout << "File \'output.txt\' was written!\n";
                }
            }
        } else if(buf == 2){
            while(true){
                std::cout << "Mix: Choose mode (0 - exit, 1 - parameters only, 2 - generate x, 3 - generate density): ";
                buf = inputIntBorders(0, 3);
                if(buf == 0){
                    break;
                } else if(buf == 1){
                    std::cout << "Mix: Input shape coefficient for 1-st dist: ";
                    buf_mp->params[0].dp->v = inputDouble();
                    std::cout << "Mix: Input shear coefficient for 1-st dist: ";
                    buf_mp->params->u = inputDouble();
                    std::cout << "Mix: Input scale coefficient for 1-st dist: ";
                    buf_mp->params->a = inputDouble();

                    std::cout << "Mix: Input shape coefficient for 2-nd dist: ";
                    (buf_mp->params + 1)->dp->v = inputDouble();
                    std::cout << "Mix: Input shear coefficient for 2-nd dist: ";
                    (buf_mp->params + 1)->u = inputDouble();
                    std::cout << "Mix: Input scale coefficient for 2-nd dist: ";
                    (buf_mp->params + 1)->a = inputDouble();

                    buf_d = mix_compute_mat_expectation(buf_mp, &ms);
                    std::cout << "Mix: Mat-Expectation: " << ((ms == MIX_SUCCESS) ? std::to_string(buf_d): "Doesn\'t exist") << '\n';
                    buf_d = mix_compute_dispersion(buf_mp, &ms);
                    std::cout << "Mix: Dispersion: " << ((ms == MIX_SUCCESS) ? std::to_string(buf_d): "Doesn\'t exist") << '\n';
                    buf_d = mix_compute_skewness(buf_mp, &ms);
                    std::cout << "Mix: Skewness: " << ((ms == MIX_SUCCESS) ? std::to_string(buf_d): "Doesn\'t exist\n") << '\n';
                    buf_d = mix_compute_excess(buf_mp, &ms);
                    std::cout << "Mix: Excess: " << ((ms == MIX_SUCCESS) ? std::to_string(buf_d): "Doesn\'t exist\n") << '\n';

                } else if(buf == 2){
                    std::cout << "Mix: Input shape coefficient for 1-st dist: ";
                    buf_mp->params[0].dp->v = inputDouble();
                    std::cout << "Mix: Input shear coefficient for 1-st dist: ";
                    buf_mp->params->u = inputDouble();
                    std::cout << "Mix: Input scale coefficient for 1-st dist: ";
                    buf_mp->params->a = inputDouble();
                    std::cout << "Mix: Input partial coefficient for 1-st dist: ";
                    buf_mp->coefs[0] = inputDouble();

                    std::cout << "Mix: Input shape coefficient for 2-nd dist: ";
                    (buf_mp->params + 1)->dp->v = inputDouble();
                    std::cout << "Mix: Input shear coefficient for 2-nd dist: ";
                    (buf_mp->params + 1)->u = inputDouble();
                    std::cout << "Mix: Input scale coefficient for 2-nd dist: ";
                    (buf_mp->params + 1)->a = inputDouble();
                    std::cout << "Mix: Input partial coefficient for 2-nd dist: ";
                    buf_mp->coefs[1] = inputDouble();

                    std::cout << "Mix: Input x count: ";
                    buf = inputInt();

                    std::ofstream file("output.txt", std::ios::trunc);
                    for(int i = 0; i < buf; i++){
                        buf_d = mix_generate_x(buf_mp, &ms);
                        if(ms != MIX_SUCCESS) break;

                        file << std::to_string(buf_d) << ' ';
                    }
                    file.close();

                    std::cout << "File \'output.txt\' was written!\n";
                } else if(buf == 3){
                    std::cout << "Mix: Input shape coefficient for 1-st dist: ";
                    buf_mp->params[0].dp->v = inputDouble();
                    std::cout << "Mix: Input shear coefficient for 1-st dist: ";
                    buf_mp->params->u = inputDouble();
                    std::cout << "Mix: Input scale coefficient for 1-st dist: ";
                    buf_mp->params->a = inputDouble();
                    std::cout << "Mix: Input partial coefficient for 1-st dist: ";
                    buf_mp->coefs[0] = inputDouble();

                    std::cout << "Mix: Input shape coefficient for 2-nd dist: ";
                    (buf_mp->params + 1)->dp->v = inputDouble();
                    std::cout << "Mix: Input shear coefficient for 2-nd dist: ";
                    (buf_mp->params + 1)->u = inputDouble();
                    std::cout << "Mix: Input scale coefficient for 2-nd dist: ";
                    (buf_mp->params + 1)->a = inputDouble();
                    std::cout << "Mix: Input partial coefficient for 2-nd dist: ";
                    buf_mp->coefs[1] = inputDouble();

                    std::cout << "Mix: Input x count: ";
                    buf = inputInt();

                    std::cout << "Mix: Input start x: ";
                    buf_d = inputDouble();
                    std::cout << "Mix: Input end x: ";
                    buf_d1 = inputDouble();

                    std::ofstream file("output.txt", std::ios::trunc);
                    for(double i = buf_d; i < buf_d1; i+=(buf_d1 - buf_d)/buf){
                        buf_d2 = mix_compute_density(i, buf_mp, &ms);
                        if(ms != MIX_SUCCESS) break;

                        file << std::to_string(buf_d2) << ' ';
                    }
                    file.close();

                    std::cout << "File \'output.txt\' was written!\n";
                }
            }
        } else if(buf == 3){
            while(true){
                std::cout << "Empirical: Choose mode (0 - exit, 1 - parameters only, 2 - generate x, 3 - generate density): ";
                buf = inputIntBorders(0, 3);
                if(buf == 0){
                    break;
                } else if(buf == 1){
                    std::cout << "Empirical: Input x count: ";
                    buf = inputInt();
                    
                    buf_darr = (double *) malloc(sizeof(double) * buf);
                    std::cout << "Empirical: Input xses: ";
                    for(int i = 0; i < buf; i++) buf_darr[i] = inputDouble();
                    
                    buf_ep = new_Empirical_p(buf_darr, buf);

                    buf_d = empirical_compute_mat_expectation(buf_ep, &es);
                    std::cout << "Empirical: Mat-Expectation: " << ((es == EMPIRICAL_SUCCESS) ? std::to_string(buf_d): "Doesn\'t exist") << '\n';
                    buf_d = empirical_compute_dispersion(buf_ep, &es);
                    std::cout << "Empirical: Dispersion: " << ((es == EMPIRICAL_SUCCESS) ? std::to_string(buf_d): "Doesn\'t exist") << '\n';
                    buf_d = empirical_compute_skewness(buf_ep, &es);
                    std::cout << "Empirical: Skewness: " << ((es == EMPIRICAL_SUCCESS) ? std::to_string(buf_d): "Doesn\'t exist\n") << '\n';
                    buf_d = empirical_compute_excess(buf_ep, &es);
                    std::cout << "Empirical: Excess: " << ((es == EMPIRICAL_SUCCESS) ? std::to_string(buf_d): "Doesn\'t exist\n") << '\n';

                    free(buf_darr);
                } else if(buf == 2){
                    std::cout << "Empirical: Input x count: ";
                    buf = inputInt();
                    
                    buf_darr = (double *) malloc(sizeof(double) * buf);
                    std::cout << "Empirical: Input xses: ";
                    for(int i = 0; i < buf; i++) buf_darr[i] = inputDouble();
                    
                    buf_ep = new_Empirical_p(buf_darr, buf);

                    std::cout << "Empirical: Input x count: ";
                    buf = inputInt();

                    std::ofstream file("output.txt", std::ios::trunc);
                    for(int i = 0; i < buf; i++){
                        buf_d = empirical_generate_x(buf_ep, &es);
                        if(es != EMPIRICAL_SUCCESS) break;

                        file << std::to_string(buf_d) << ' ';
                    }
                    file.close();

                    std::cout << "File \'output.txt\' was written!\n";
                    free(buf_darr);
                } else if(buf == 3){
                    std::cout << "Empirical: Input x count: ";
                    buf = inputInt();
                    
                    buf_darr = (double *) malloc(sizeof(double) * buf);
                    std::cout << "Empirical: Input xses: ";
                    for(int i = 0; i < buf; i++) buf_darr[i] = inputDouble();
                    
                    buf_ep = new_Empirical_p(buf_darr, buf);

                    std::cout << "Empirical: Input x count: ";
                    buf = inputInt();

                    std::cout << "Mix: Input start x: ";
                    buf_d = inputDouble();
                    std::cout << "Mix: Input end x: ";
                    buf_d1 = inputDouble();

                    std::ofstream file("output.txt", std::ios::trunc);
                    for(double i = buf_d; i < buf_d1; i+=(buf_d1 - buf_d)/buf){
                        buf_d2 = empirical_compute_density(i, buf_ep, &es);
                        if(es != EMPIRICAL_SUCCESS) break;

                        file << std::to_string(buf_d2) << ' ';
                    }
                    file.close();

                    std::cout << "File \'output.txt\' was written!\n";

                    free(buf_darr);
                }
            }
        }
    }
    

    return 0;
}