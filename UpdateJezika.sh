cat << EOF
lupdate -verbose KasaQT.pro
linguist languages/lang_hr.ts languages/lang_en.ts
lrelease KasaQT.pro
EOF
