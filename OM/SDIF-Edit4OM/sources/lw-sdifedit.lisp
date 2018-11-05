;;; SDIF-Edit4OM
;;; Use SDIF-Edit as an editor for SDIF files in OpenMusic
;;; Jean Bresson, Ircam 2010

;;===========================================
; CONNECTION SDIFEDIT USING LISPWORKS
;;===========================================

(in-package :om)

(defun restore-app-path (path)
  (let ((pathstr (namestring path)))
    (when (equal (elt pathstr (- (length pathstr) 1)) #\/)
      (setf pathstr (subseq pathstr 0 (- (length pathstr) 1)))
      (let ((appname (pathname-name (pathname (subseq pathstr 0 (- (length pathstr) 1))))))
        (setf pathstr (namestring (make-pathname :directory (append (pathname-directory path) 
                                                                    (list "Contents" "MacOS"))
                                                 :name appname)))))
    pathstr))

(defvar *sdif-edit-path* nil)
(setf *sdif-edit-path* "/Applications/SDIF-Edit")

(defun om-run-sdifedit (ref filepath &optional audio print-output)
   (if (and *sdif-edit-path* (probe-file *sdif-edit-path*))
       (let ((exepath (restore-app-path *sdif-edit-path*)))
         (mp:process-run-function (concatenate 'string "SDIF-Edit--" filepath) nil
                                 #'(lambda ()
                                     ;(system::run-shell-command (print (format nil "~s ~s" exepath filepath)) :wait t)
                                     (om-cmd-line (print (format nil "~s ~s" exepath filepath)) print-output)
                                     ))
         )
     (om-message-dialog "SDIF-Edit not available !"))
   )

(defun om-sdifedit-open-p (objref)
  (mp::find-process-from-name (concatenate 'string "SDIF-Edit--" (string objref))))

(defun om-select-sdifedit-window (objref)
  (system::call-system (concatenate 'string "open " (namestring *sdif-edit-path*))))

