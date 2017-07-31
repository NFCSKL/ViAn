#include "analysisdialog.h"

AnalysisDialog::AnalysisDialog()
{

    QVBoxLayout* v_lay = new QVBoxLayout();
    v_lay->addWidget(new QListWidget());

    // Add analysis form
    QFormLayout* form_lay = new QFormLayout();

    QLineEdit* sample_freq = new QLineEdit("1");
    QLineEdit* interval = new QLineEdit("100%");
    form_lay->addRow("Sample frequency", sample_freq);
    form_lay->addRow("Analysed interval", interval);
    v_lay->addLayout(form_lay);
    // Add analysis specific options
    dilation = new QLineEdit("16");
    small_obj = new QLineEdit("500");
    det_tresh = new QLineEdit("25");

    graywhite = new QLineEdit("255");
    bg_hist = new QLineEdit("500");
    mog2_thresh = new QLineEdit("50");
    det_shadows = new QLineEdit("NO");

    form_lay->addRow("Dilation degree: ", dilation);
    form_lay->addRow("Smallest object size: ", small_obj);
    form_lay->addRow("Detection threshhold: ", det_tresh);
    form_lay->addRow("Grayscale white: ", graywhite);
    form_lay->addRow("Background history: ", bg_hist);

    form_lay->addRow("MOG2 threshhold: ", mog2_thresh);
    form_lay->addRow("Detect shadows: ", det_shadows);

    // Add Buttons
    QDialogButtonBox* btn_box = new QDialogButtonBox;
    btn_box->addButton(QDialogButtonBox::Ok);
    btn_box->addButton(QDialogButtonBox::Cancel);
    v_lay->addWidget(btn_box);
    setLayout(v_lay);
}
