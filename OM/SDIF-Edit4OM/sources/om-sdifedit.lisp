;;; SDIF-Edit4OM
;;; Use SDIF-Edit as an editor for SDIF files in OpenMusic
;;; Jean Bresson, Ircam 2010

(in-package :om)

(defvar *use-sdif-edit* t)

;;; Main method redefinition
(defmethod OpenObjectEditor ((self sdifFilebox))
   (if *use-sdif-edit*
       (if (filepathname (value self))
           (let* ((file (namestring (filepathname (value self))))
                  (open? (om-sdifedit-open-p file)))
             (if  open?
                 (om-select-sdifedit-window file)
               (om-run-sdifedit file file)))
     (om-message-dialog "This SDIF box has no attached file!"))
   (call-next-method)))


;;;=====================
;;; SDIF-EDIT PREFS

(pushr 'sdifedit *external-prefs*)

(defmethod get-external-name ((module (eql 'sdifedit))) "SDIF-Edit")

(defmethod get-external-module-vals ((module (eql 'sdifedit)) modulepref) (get-pref modulepref :sdifedit-options))
(defmethod get-external-module-path ((module (eql 'sdifedit)) modulepref) (get-pref modulepref :sdifedit-path))
(defmethod set-external-module-vals ((module (eql 'sdifedit)) modulepref vals) (set-pref modulepref :sdifedit-options vals))
(defmethod set-external-module-path ((module (eql 'sdifedit)) modulepref path) 
  (set-pref modulepref :sdifedit-path path))


(defmethod get-external-def-vals ((module (eql 'sdifedit))) 
  (list :sdifedit-path (om-external-app nil "SDIF-Edit")
        :sdifedit-options '(t)))
 
(defmethod save-external-prefs ((module (eql 'sdifedit))) 
  `(:sdifedit-path ,(om-save-pathname *sdif-edit-path*) 
    :sdifedit-options (list ,*use-sdif-edit*)))


(defmethod put-external-preferences ((module (eql 'sdifedit)) moduleprefs)
  (let ((list-prefs (get-pref moduleprefs :sdifedit-options)))
    (when list-prefs 
      (setf *use-sdif-edit* (car list-prefs)))
    (when (get-pref moduleprefs :sdifedit-path)
      (setf *sdif-edit-path* (get-pref moduleprefs :sdifedit-path)))
    ))


(defmethod show-external-prefs-dialog ((module (eql 'sdifedit)) prefvals)
  (let* ((rep-list (copy-list prefvals))
         (dialog (om-make-window 'om-dialog
                                 :window-title "SDIF-Edit Options"
                                 :size (om-make-point 210 120)
                                 :position :centered
                                 :resizable nil :maximize nil :close nil))
         (pos 20)
         (use-or-not (om-make-dialog-item 'om-check-box  
                                         (om-make-point 20 pos) (om-make-point 180 17) "Use SDIF-Edit"
                                         :checked-p (car rep-list)
                                         :font *om-default-font2*)))

    
    (setf pos (+ pos 40))
    (om-add-subviews dialog use-or-not
                     
     
      (om-make-dialog-item 'om-button (om-make-point 10 pos) (om-make-point 80 20) "Cancel"
                           :di-action (om-dialog-item-act item
                                        (om-return-from-modal-dialog dialog nil)))
      
      (om-make-dialog-item 'om-button (om-make-point 100 pos) (om-make-point 80 20) "OK"
                           :di-action (om-dialog-item-act item
                                        (setf (car rep-list) (om-checked-p use-or-not))
                                        (om-return-from-modal-dialog dialog rep-list))
                           :default-button t :focus t)
      )
    (om-modal-dialog dialog)))


