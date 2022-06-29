package ro.ase.ism.Radu.Gigi;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.security.*;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.security.spec.PKCS8EncodedKeySpec;

import javax.crypto.*;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
public class RaduGigiExam {
    public static String byte2Hex(byte[] array) {
        String output = "";
        for(byte value : array) {
            output += String.format("%02X", value);
        }
        return output;
    }

    public static void main(String[] args) {
        try {
            // 1
            File msg = new File("msg.txt");
            FileInputStream fis = new FileInputStream(msg);
            MessageDigest md = MessageDigest.getInstance("SHA-256");
            byte[] buffer = new byte[1];
            while ((fis.read(buffer)) != -1) {
                md.update(buffer);
            }
            byte[] hashValue = md.digest();
            System.out.println("SHA256: " + byte2Hex(hashValue));


            // 2
            File msg_enc = new File("enc_msg.aes");
            if (!msg_enc.exists()) {
                msg_enc.createNewFile();
            }
            FileOutputStream fos = new FileOutputStream(msg_enc);
            fis = new FileInputStream(msg);

            Cipher c = Cipher.getInstance("AES/CBC/PKCS5Padding");
            int size = c.getBlockSize();
            byte[] IV = new byte[size];
            IV[5] = (byte) 0xCC;
            //System.out.println(byte2Hex(IV));

            byte[] key = "passwordsecurity".getBytes();
            SecretKeySpec keySpec = new SecretKeySpec(key, "AES");
            IvParameterSpec ivSpec = new IvParameterSpec(IV);
            c.init(Cipher.ENCRYPT_MODE, keySpec, ivSpec);

            byte[] in = new byte[size];
            byte[] out;
            int bytes;
            while ((bytes = fis.read(in)) != -1) {
                out = c.update(in, 0, bytes);
                fos.write(out);
            }
            out = c.doFinal();
            fos.write(out);

            fis.close();
            fos.close();

            // 3
            byte[] priv_key = Files.readAllBytes(Paths.get("priv_key"));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
