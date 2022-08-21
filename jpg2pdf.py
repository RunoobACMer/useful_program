import fitz  
import os

img_path = 'D:/Pictures/kaoyan'
doc = fitz.open()

for _, _, files in os.walk(img_path):
    for file in files:
        jpg_img = file
        jpg_file = os.path.join(img_path, file)
        imgdoc = fitz.open(jpg_file)
        pdfbytes = imgdoc.convert_to_pdf()
        pdf_name = os.path.join(img_path, file[:-4] + '.pdf')


        imgpdf = fitz.open(pdf_name, pdfbytes)
        doc.insert_pdf(imgpdf)
doc.save('combined.pdf')
doc.close()
