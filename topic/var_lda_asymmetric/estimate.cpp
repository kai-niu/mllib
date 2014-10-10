// Copyright 2013 lijiankou. All Rights Reserved.
// Author: lijk_start@163.com (Jiankou Li)
#include "base_head.h"
#include "lda.h"
#include "lda_var_em.h"

using namespace ml;

DEFINE_string(cor_path, "/data0/data/lda_user_sku_20-40_5000", "");
DEFINE_int32(topic_num, 10, "");
DEFINE_double(alpha, 0.01, "");

void LdaApp() {
  long t1;
  (void) time(&t1);
  seedMT(t1);

  float em_converged = 1e-4;
  int em_max_iter = 20;
  int em_estimate_alpha = 1; //1 indicate estimate alpha and 0 use given value
  int var_max_iter = 30;
  double var_converged = 1e-6;
  int n_topic = FLAGS_topic_num;

  Corpus cor;
  Str data = FLAGS_cor_path;
  cor.LoadData(data);
  Corpus train;
  Corpus test;
  double p = 0.8;
  SplitData(cor, p, &train, &test);
  Str type = "seeded";  
  
  LdaModel m;
  double* alpha = new double[n_topic];
  for(int k=0; k<n_topic; ++k) {
    alpha[k] = 0.01;
  }
  m.InitModel(n_topic,train.num_terms,alpha);
  
  LDA lda;
  lda.Init(em_converged, em_max_iter, em_estimate_alpha, var_max_iter,var_converged);  
  lda.RunEM(type, train, test, &m);

  LOG(INFO) << m.alpha;
  VVReal gamma;
  VVVReal phi;
  lda.Infer(test, m, &gamma, &phi);
  WriteStrToFile(Join(gamma, " ", "\n"), "gamma");
  WriteStrToFile(Join(phi, " ", "\n", "\n\n"), "phi");
}

int main(int argc, char* argv[]) {
  ::google::ParseCommandLineFlags(&argc, &argv, true);
  LdaApp();

  return 1; 
}
